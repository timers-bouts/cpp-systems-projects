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

int main() {

    telemetry_sim::TelemetrySimulator::Config config;
    config.packet_count = 20;
    telemetry::TelemetryRecorder recorder("simulation.bin", telemetry::TelemetryRecorder::OpenMode::Truncate);
    telemetry_sim::TelemetrySimulator simulator(config, recorder);
    simulator.run();
    auto reader = telemetry::TelemetryReader::open("simulation.bin");
    std::vector<uint8_t> payload;
    while (reader.read_next(payload)) {
        telemetry_sim::TelemetryFrame frame;
        telemetry::PacketReader pr(payload);
        frame.deserialize(pr);
        print_frame(frame);
    }

    return 0;
}
