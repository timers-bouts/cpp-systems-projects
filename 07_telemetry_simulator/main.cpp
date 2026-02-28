#include <cstdint>
#include <iostream>
#include <vector>
#include <span>

#include "telemetry_sim/TelemetryFrame.h"
#include "telemetry_sim/TelemetrySimulator.h"
#include "telemetry/TelemetryReader.h"
#include "telemetry/TelemetryRecorder.h"
#include "telemetry_sim/TelemetryFrame.h"

void print_frame(telemetry_sim::TelemetryFrame& frame) {
    std::cout << "-------------Telemetry Readings-------------\n";
    std::cout << "Timestamp: " << frame.timestamp_ms << "\n";
    std::cout << "Temperature: " << frame.temperature_c << " degress Celsius\n";
    std::cout << "Voltage: " << frame.voltage_v << " Volts\n";
    std::cout << "Position X: " << frame.position_x << " Meters\n";
    std::cout << "Position Y: " << frame.position_y << " Meters\n";
    std::cout << "Velocity: " << frame.velocity_mps << " Meters per Second\n";
    std::cout << "Status Flags: " << frame.status_flags << "\n";
    std::cout << "-------------End of Readings-------------\n\n\n";

}

void print_frame_table( std::span<const telemetry_sim::TelemetryFrame> frames) {
    // Print header
    std::cout << std::setw(10) << "Time(ms)"
              << std::setw(12) << "Temp(C)"
              << std::setw(10) << "Volt(V)"
              << std::setw(12) << "PosX(m)"
              << std::setw(12) << "PosY(m)"
              << std::setw(12) << "Vel(m/s)"
              << std::setw(10) << "Flags"
              << "\n";

    std::cout << std::string(78, '-') << "\n";

    // Print each frame
    for (const auto& f : frames) {
        std::cout << std::setw(10) << f.timestamp_ms
                  << std::setw(12) << f.temperature_c
                  << std::setw(10) << f.voltage_v
                  << std::setw(12) << f.position_x
                  << std::setw(12) << f.position_y
                  << std::setw(12) << f.velocity_mps
                  << std::setw(10) << f.status_flags
                  << "\n";
    }
}

int main() {

    telemetry::Logger logger("sim.log", telemetry::LogLevel::Info, false);
    telemetry_sim::TelemetrySimulator::Config config;
    config.packet_count = 20;
    telemetry::TelemetryRecorder recorder("simulation.bin", logger,
                        telemetry::TelemetryRecorder::OpenMode::Truncate);
    telemetry_sim::TelemetrySimulator simulator(config, recorder, logger, true);
    simulator.run();
    auto frames = simulator.get_frames();
    print_frame_table(frames);

    return 0;
}
