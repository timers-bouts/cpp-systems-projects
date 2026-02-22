#include "TelemetryFrame.h"

namespace telemetry_sim {

    void TelemetryFrame::serialize(telemetry::PacketWriter& writer) const {
        // Serialize fields in this order for consistent binary format
        writer.add_u64(timestamp_ms);
        writer.add_float(temperature_c);
        writer.add_float(voltage_v);
        writer.add_float(position_x);
        writer.add_float(position_y);
        writer.add_float(velocity_mps);
        writer.add_u32(status_flags);

        std::uint32_t crc = crc32(writer.bytes());
        writer.add_u32(crc);

    };

    std::uint32_t TelemetryFrame::crc32(const std::span<const uint8_t>& data) const {
        std::uint32_t polynomial = 0x04C11DB7;
        std::uint32_t crc = 0xFFFFFFFF;

        for (std::uint8_t byte : data) {
            byte ^= static_cast<uint8_t>((crc >> 24) & 0xFF);
            crc = (crc & 0x00FFFFFF) | (static_cast<uint32_t>(byte) << 24);

            for (int i = 0; i < 8; i++) {
                if (crc & 0x80000000) {
                    crc = (crc << 1) ^ polynomial;
                } else {
                    crc <<= 1;
                }
            }
        }

        crc ^= 0xFFFFFFFF;
        return crc;
    }

}