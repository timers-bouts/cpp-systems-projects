#include <iostream>
#include <cstdint>
#include <vector>
#include <iomanip>

#include "telemetry/PacketWriter.h"

void print_packet(const std::span<const uint8_t>& bytes) {
    std::cout << std::hex; // Change stdout to write in hex
    for (uint8_t byte : bytes) {
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
    PacketWriter pw{Endianness::Big};

    // 1) Message type (u8)
    pw.add_u8(0x01);

    // 2) Sequence number (u16)
    pw.add_u16(42);

    // 3) Timestamp (u32)
    pw.add_u32(100000);

    // 4) Temperature (float)
    pw.add_float(25.5f);

    // 5) Device ID (string length-prefixed)
    pw.add_string("SENSOR_A");

    // Print packet bytes in hex
    print_packet(pw.bytes());

    return 0;
}
