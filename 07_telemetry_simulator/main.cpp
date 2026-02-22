#include <cstdint>
#include <iostream>
#include <vector>
#include <span>

#include "telemetry_sim/TelemetryFrame.h"
#include "telemetry_sim/TelemetrySimulator.h"
#include "telemetry/TelemetryReader.h"
#include "telemetry/TelemetryRecorder.h"
#include "telemetry_sim/TelemetryFrame.h"

void print_packet(const std::span<const std::uint8_t>& bytes) {
    std::cout << std::hex; // Change stdout to write in hex
    for (std::uint8_t byte : bytes) {
        std::cout << std::setw(2)
                << std::uppercase
                << std::setfill('0')
                << static_cast<int>(byte)
                << " ";
    }
    std::cout << "\n";
    std::cout << std::dec; // change stdout back to decimal mode

}

int main() {

    telemetry_sim::TelemetrySimulator::Config config;
    config.packet_count = 20;
    telemetry::TelemetryRecorder recorder("test.bin", telemetry::TelemetryRecorder::OpenMode::Truncate);
    telemetry_sim::TelemetrySimulator simulator(config, recorder);
    simulator.run();
telemetry::TelemetryReader reader("test.bin");
std::vector<uint8_t> pkt;
while (reader.read_next(pkt)) {
    telemetry::PacketReader pr(pkt); // hypothetical PacketReader
    TelemetryFrame frame;
    frame.deserialize(pr);           // you’d implement this method
    std::cout << frame.timestamp_ms << " "
              << frame.temperature_c << " "
              << frame.voltage_v << " "
              << frame.position_x << " "
              << frame.position_y << " "
              << frame.velocity_mps << "\n";
}

    return 0;
}
