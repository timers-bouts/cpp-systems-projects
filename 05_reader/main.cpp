#include <telemetry/TelemetryReader.h>
#include <telemetry/TelemetryFormat.h>
#include <telemetry/CRC.h>

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ------------------------------
// Tiny binary writing helpers
// ------------------------------
static void write_u16_le(std::ostream& out, std::uint16_t v) {
    std::array<std::uint8_t, 2> b{
        static_cast<std::uint8_t>(v & 0xFF),
        static_cast<std::uint8_t>((v >> 8) & 0xFF),
    };
    out.write(reinterpret_cast<const char*>(b.data()), static_cast<std::streamsize>(b.size()));
}

static void write_u32_le(std::ostream& out, const std::uint32_t v) {
    std::array<std::uint8_t, 4> b{
        static_cast<std::uint8_t>(v & 0xFF),
        static_cast<std::uint8_t>((v >> 8) & 0xFF),
        static_cast<std::uint8_t>((v >> 16) & 0xFF),
        static_cast<std::uint8_t>((v >> 24) & 0xFF),
    };
    out.write(reinterpret_cast<const char*>(b.data()), static_cast<std::streamsize>(b.size()));
}

static void write_header(std::ostream& out, std::uint16_t version, std::uint16_t flags) {
    // Magic
    out.write(reinterpret_cast<const char*>(telemetry::format::kMagic.data()),
              static_cast<std::streamsize>(telemetry::format::kMagic.size()));
    // Version + flags
    write_u16_le(out, version);
    write_u16_le(out, flags);
}

static void write_crc(std::ostream& out, const std::uint32_t crc) {
    write_u32_le(out, crc);
}

static void write_record(std::ostream& out, const std::vector<std::uint8_t>& payload) {
    write_u32_le(out, static_cast<std::uint32_t>(payload.size() + telemetry::format::CRC32_SIZE));
    if (!payload.empty()) {
        out.write(reinterpret_cast<const char*>(payload.data()),
                  static_cast<std::streamsize>(payload.size()));
    }
    write_crc(out, telemetry::crc32(std::span<const uint8_t>(payload)));
}

// ------------------------------
// Mini test framework
// ------------------------------
struct TestResult {
    int passed = 0;
    int failed = 0;
};

static void pass(TestResult& tr, const std::string& name) {
    ++tr.passed;
    std::cout << "[PASS] " << name << "\n";
}

static void fail(TestResult& tr, const std::string& name, const std::string& why) {
    ++tr.failed;
    std::cout << "[FAIL] " << name << " — " << why << "\n";
}

template <typename Fn>
static bool expect_parse_error(Fn&& fn, std::string* out_msg = nullptr) {
    try {
        fn();
        return false;
    } catch (const telemetry::ParseError& e) {
        if (out_msg) *out_msg = e.what();
        return true;
    }
}

// ------------------------------
// Tests
// ------------------------------

static void test_happy_path_dump(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "happy_path_reads_all_packets";
    const fs::path file = dir / "happy.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, telemetry::format::kCurrentVersion, 0);

        write_record(out, {0x01});
        write_record(out, {0x10, 0x11, 0x12, 0x13});
        write_record(out, std::vector<std::uint8_t>(20, 0xAB)); // 20 bytes of 0xAB
    }

    try {
        auto r = telemetry::TelemetryReader::open(file, logger);
        std::vector<std::uint8_t> pkt;

        int count = 0;
        std::vector<std::size_t> sizes;
        while (r.read_next(pkt)) {
            ++count;
            sizes.push_back(pkt.size());
        }

        if (count != 3) {
            fail(tr, name, "expected 3 packets, got " + std::to_string(count));
            return;
        }
        if (!(sizes.size() == 3 && sizes[0] == 1 && sizes[1] == 4 && sizes[2] == 20)) {
            fail(tr, name, "packet sizes mismatch");
            return;
        }

        pass(tr, name);
    } catch (const std::exception& e) {
        fail(tr, name, e.what());
    }
}

static void test_clean_eof_behavior(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "clean_eof_returns_false_not_throw";
    const fs::path file = dir / "clean_eof.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, telemetry::format::kCurrentVersion, 0);
        write_record(out, {0xAA, 0xBB});
    }

    try {
        auto r = telemetry::TelemetryReader::open(file, logger);
        std::vector<std::uint8_t> pkt;

        bool first = r.read_next(pkt);
        bool second = r.read_next(pkt);
        bool third = r.read_next(pkt); // should also be false

        if (!first) {
            fail(tr, name, "expected first read_next to return true");
            return;
        }
        if (second) {
            fail(tr, name, "expected second read_next to return false at EOF");
            return;
        }
        if (third) {
            fail(tr, name, "expected third read_next to remain false at EOF");
            return;
        }

        pass(tr, name);
    } catch (const std::exception& e) {
        fail(tr, name, e.what());
    }
}

static void test_truncated_header(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "truncated_header_rejected";
    const fs::path file = dir / "truncated_header.bin";

    // Write fewer than 8 bytes (header is 8 bytes)
    {
        std::ofstream out(file, std::ios::binary);
        const char bytes[3] = {'T', 'L', 'R'};
        out.write(bytes, 3);
    }

    std::string msg;
    const bool threw = expect_parse_error([&] {
        (void)telemetry::TelemetryReader::open(file, logger);
    }, &msg);

    if (!threw) {
        fail(tr, name, "expected ParseError, got no exception");
        return;
    }
    pass(tr, name);
}

static void test_invalid_magic(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "invalid_magic_rejected";
    const fs::path file = dir / "bad_magic.bin";

    {
        std::ofstream out(file, std::ios::binary);
        // Wrong magic
        const char bad_magic[4] = {'B','A','D','!'};
        out.write(bad_magic, 4);
        write_u16_le(out, telemetry::format::kCurrentVersion);
        write_u16_le(out, 0);
    }

    const bool threw = expect_parse_error([&] {
        (void)telemetry::TelemetryReader::open(file, logger);
    });

    if (!threw) {
        fail(tr, name, "expected ParseError, got no exception");
        return;
    }
    pass(tr, name);
}

static void test_unsupported_version(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "unsupported_version_rejected";
    const fs::path file = dir / "bad_version.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, /*version=*/static_cast<std::uint16_t>(telemetry::format::kCurrentVersion + 1), /*flags=*/0);
    }

    const bool threw = expect_parse_error([&] {
        (void)telemetry::TelemetryReader::open(file, logger);
    });

    if (!threw) {
        fail(tr, name, "expected ParseError, got no exception");
        return;
    }
    pass(tr, name);
}

static void test_truncated_size_field(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "truncated_size_field_throws";
    const fs::path file = dir / "truncated_size.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, telemetry::format::kCurrentVersion, 0);
        // Write only 2 bytes of the 4-byte size field
        const std::array<std::uint8_t, 2> partial = {0x05, 0x00};
        out.write(reinterpret_cast<const char*>(partial.data()), 2);
    }

    try {
        auto r = telemetry::TelemetryReader::open(file, logger);
        std::vector<std::uint8_t> pkt;
        (void)r.read_next(pkt);
        fail(tr, name, "expected ParseError, got no exception");
    } catch (const telemetry::ParseError&) {
        pass(tr, name);
    } catch (const std::exception& e) {
        fail(tr, name, std::string("unexpected exception: ") + e.what());
    }
}

static void test_truncated_payload(TestResult& tr, const fs::path& dir, telemetry::Logger& logger) {
    const std::string name = "truncated_payload_throws";
    const fs::path file = dir / "truncated_payload.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, telemetry::format::kCurrentVersion, 0);
        // size says 10
        write_u32_le(out, 10);
        // but only write 3 payload bytes
        const std::array<std::uint8_t, 3> payload = {0xDE, 0xAD, 0xBE};
        out.write(reinterpret_cast<const char*>(payload.data()), 3);
    }

    try {
        auto r = telemetry::TelemetryReader::open(file, logger);
        std::vector<std::uint8_t> pkt;
        (void)r.read_next(pkt);
        fail(tr, name, "expected ParseError, got no exception");
    } catch (const telemetry::ParseError&) {
        pass(tr, name);
    } catch (const std::exception& e) {
        fail(tr, name, std::string("unexpected exception: ") + e.what());
    }
}

static void test_oversized_packet_rejected(TestResult& tr, const fs::path& dir,
                                            telemetry::Logger& logger) {
    const std::string name = "oversized_packet_rejected_before_alloc";
    const fs::path file = dir / "oversized.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, telemetry::format::kCurrentVersion, 0);
        // size is huge (over default max 64KiB)
        write_u32_le(out, 1024 * 1024); // 1 MiB
        // no payload needed
    }

    try {
        telemetry::TelemetryReader::Options opt;
        opt.max_packet_size = 64 * 1024;

        auto r = telemetry::TelemetryReader::open(file, opt, logger);
        std::vector<std::uint8_t> pkt;
        (void)r.read_next(pkt);
        fail(tr, name, "expected ParseError, got no exception");
    } catch (const telemetry::ParseError&) {
        pass(tr, name);
    } catch (const std::exception& e) {
        fail(tr, name, std::string("unexpected exception: ") + e.what());
    }
}

static void test_zero_size_packet_rejected(TestResult& tr, const fs::path& dir,
                                            telemetry::Logger& logger) {
    const std::string name = "zero_size_packet_rejected";
    const fs::path file = dir / "zero_size.bin";

    {
        std::ofstream out(file, std::ios::binary);
        write_header(out, telemetry::format::kCurrentVersion, 0);
        write_u32_le(out, 0);
    }

    try {
        auto r = telemetry::TelemetryReader::open(file, logger);
        std::vector<std::uint8_t> pkt;
        (void)r.read_next(pkt);
        fail(tr, name, "expected ParseError, got no exception");
    } catch (const telemetry::ParseError&) {
        pass(tr, name);
    } catch (const std::exception& e) {
        fail(tr, name, std::string("unexpected exception: ") + e.what());
    }
}

// ------------------------------
// main()
// ------------------------------
int main() {
    TestResult tr;
    telemetry::Logger logger("test.log", telemetry::LogLevel::Info, false);

    const fs::path dir = fs::path("test_artifacts");
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir, ec);

    std::cout << "Running TelemetryReader tests...\n\n";

    test_happy_path_dump(tr, dir, logger);
    test_clean_eof_behavior(tr, dir, logger);
    test_truncated_header(tr, dir, logger);
    test_invalid_magic(tr, dir, logger);
    test_unsupported_version(tr, dir, logger);
    test_truncated_size_field(tr, dir, logger);
    test_truncated_payload(tr, dir, logger);
    test_oversized_packet_rejected(tr, dir, logger);
    test_zero_size_packet_rejected(tr, dir, logger);

    std::cout << "\nSummary: " << tr.passed << " passed, " << tr.failed << " failed\n";
    return (tr.failed == 0) ? 0 : 1;
}
