#include <stdexcept>
#include <array>
#include <cstdint>
#include <limits>
#include "telemetry/TelemetryRecorder.h"

namespace telemetry {

    TelemetryRecorder::TelemetryRecorder(const std::filesystem::path& path, OpenMode mode) {

        if (mode == TelemetryRecorder::OpenMode::Append) {
            out_.open(path, std::ios::binary | std::ios::out | std::ios::app);
        } else {
            out_.open(path, std::ios::binary | std::ios::out | std::ios::trunc);
        }

        if (!out_.is_open()) {
            throw std::runtime_error("Error opening file");
        }

    }

    TelemetryRecorder::~TelemetryRecorder() = default;

    void TelemetryRecorder::write_u32_le(std::uint32_t value) {
        // Write using little endian
        std::array<uint8_t, 4> bytes = {static_cast<std::uint8_t>(value & 0xFF), static_cast<std::uint8_t>((value >> 8) & 0xFF),
                                        static_cast<std::uint8_t>((value >> 16) & 0xFF), static_cast<std::uint8_t>((value >> 24) & 0xFF)};
        out_.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(4));
    }

    void TelemetryRecorder::write_packet(std::span<const std::uint8_t> packet_bytes) {
        if (packet_bytes.size() > std::numeric_limits<std::uint32_t>::max()) {
            throw std::runtime_error("Packet size exceeds limits for uint32_t");
        }
        std::uint32_t size = static_cast<std::uint32_t>(packet_bytes.size());
        write_u32_le(size);
        out_.write(reinterpret_cast<const char*>(packet_bytes.data()), static_cast<std::streamsize>(packet_bytes.size()));
    }

}