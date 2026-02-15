#include "telemetry/PacketWriter.h"
#include "telemetry/TelemetryRecorder.h"


int main() {

    telemetry::PacketWriter pw(telemetry::PacketWriter::Endianness::Little);
    std::array<std::uint8_t, 3> raw{0xAA, 0xBB, 0xCC};
    pw.add_bytes(raw);

    telemetry::TelemetryRecorder tr("test.bin", telemetry::TelemetryRecorder::OpenMode::Append);
    tr.write_packet(pw.bytes());

    pw.clear();
    pw.add_bytes(std::array<std::uint8_t, 5>{0x12, 0x34, 0x56, 0x78, 0x9A});
    tr.write_packet(pw.bytes());

    return 0;
}