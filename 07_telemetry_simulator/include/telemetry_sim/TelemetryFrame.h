#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include "telemetry/PacketWriter.h"
#include "telemetry/PacketReader.h"


namespace telemetry_sim {

    // This struct represents one telemetry packet.
    // You can add or remove fields as needed for your simulation.
    struct TelemetryFrame {
        uint64_t timestamp_ms = 0;   // milliseconds since simulation start
        float temperature_c = 0.0f;     // degrees Celsius
        float voltage_v = 0.0f;         // volts
        float position_x = 0.0f;        // meters
        float position_y = 0.0f;       // meters
        float velocity_mps = 0.0f;      // meters per second
        uint32_t status_flags = 0;   // bitmask for error/status flags

        // Serialize this frame into a PacketWriter payload.
        // This function is small and explicit: each field is written in order.
        void serialize(telemetry::PacketWriter& writer) const;

        // Desieralize the frame to assign struct members from a packet
        void deserialize(telemetry::PacketReader& reader);

    };

} // namepsace telemetry_sim
