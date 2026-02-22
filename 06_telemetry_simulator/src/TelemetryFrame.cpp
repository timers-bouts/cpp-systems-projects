#include <random>

#include "TelemetryFrame.h"

namespace telemetry_sim {

    struct TelemetryFrame {
        uint64_t timestamp_ms = 0;
        float temperature_c = 0.0f;
        float voltage_v = 0.0f;
        float position_x = 0.0f;
        float position_y = 0.0f;
        float velocity_mps = 0.0f;
        uint32_t status_flags = 0;

        void TelemetryFrame::serialize(telemetry::PacketWriter& writer) const {
            writer.add_u64(timestamp_ms);
            writer.add_float(temperature_c);
            writer.add_float(voltage_v);
            writer.add_float(position_x);
            writer.add_float(position_y);
            writer.add_float(velocity_mps);
            writer.add_u32(status_flags);
        }

    };

}