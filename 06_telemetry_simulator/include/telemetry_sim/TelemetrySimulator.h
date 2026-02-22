#pragma once

#include <cstdint>
#include <random>
#include "TelemetryFrame.h"
#include "telemetry/TelemetryRecorder.h"

namespace telemetry_sim {

    class TelemetrySimulator {
        public:
            // Simple configuration struct
            struct Config {
                uint64_t start_time_ms = 0;   // simulation start timestamp
                uint64_t step_ms = 100;       // time per frame in ms
                size_t packet_count = 1000;   // number of packets to generate
                uint32_t seed = 42;           // RNG seed for deterministic output
            };

            // Constructor: takes configuration and a recorder instance
            TelemetrySimulator(const Config& config, telemetry::TelemetryRecorder& recorder);

            // Run the simulation: generates frames, serializes, and records them
            void run();

        private:
            // Generate a single frame based on current simulator state
            TelemetryFrame generate_frame();

            // Advance internal state (position, velocity, temperature, etc.)
            void advance_state();

            Config config_;
            telemetry::TelemetryRecorder& recorder_;

            // Random number generator
            std::mt19937 rng_;

            // Internal simulation state
            uint64_t current_time_ms_;
            float position_x_;
            float position_y_;
            float velocity_mps_;
            float temperature_c_;
            float voltage_v_;
    
    };

}
