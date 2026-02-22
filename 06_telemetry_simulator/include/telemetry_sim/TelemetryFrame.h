#pragma once

#include <cstdint>
#include "telemetry/PacketWriter.h"
#include <array>
#include <vector>

namespace telemetry_sim {

    // This struct represents one telemetry packet.
    // You can add or remove fields as needed for your simulation.
    struct TelemetryFrame {
        uint64_t timestamp_ms;   // milliseconds since simulation start
        float temperature_c;     // degrees Celsius
        float voltage_v;         // volts
        float position_x;        // meters
        float position_y;        // meters
        float velocity_mps;      // meters per second
        uint32_t status_flags;   // bitmask for error/status flags

        // Serialize this frame into a PacketWriter payload.
        // This function is small and explicit: each field is written in order.
        void serialize(telemetry::PacketWriter& writer) const;
    };

}
