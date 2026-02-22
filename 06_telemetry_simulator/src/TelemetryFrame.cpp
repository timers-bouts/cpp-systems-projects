#include "TelemetryFrame.h"
#include "telemetry/CRC.h"

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

    };

}