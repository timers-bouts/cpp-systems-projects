# Telemetry Packet Builder (C++)

This project is a reusable telemetry packet construction utility written in modern C++20.

It was built as part of a structured sequence of smaller C++ systems projects designed
to develop low-level binary programming skills before building a full telemetry packet
simulator capstone.

The packet builder supports:

- Safe byte-oriented packet construction using `std::vector<uint8_t>`
- Appending common telemetry field types (`u8`, `u16`, `u32`, floats, raw bytes)
- Explicit endianness handling (big vs little endian)
- Exporting packet data as a binary-safe buffer using `std::span`

---

## Project Structure

PacketWriter lives in ../telemetry_lib/
03_packet_builder/ is now a demo app that uses the shared library

```text
03_packet_builder/
  main.cpp
  Makefile
```

PacketWriter is now part of the shared telemetry library:
```text
telemetry_lib/include/telemetry/PacketWriter.h
telemetry_lib/src/PacketWriter.cpp
```

---

## Features
- Modern C++ systems-style API design
- Binary-safe internal storage using std::vector<std::uint8_t>
- Incremental field packing with clear byte-order control
- Support for common telemetry primitives:
- Unsigned integers (u8, u16, u32)
- Floating-point values (float)
- Raw byte payloads (add_bytes)
- Efficient buffer reuse with reserve()
- Clean modular library-style layout (header/source split)

Example packet output (hex):
```
01 00 2A 00 01 86 A0 41 CC 00 00 00 08 53 45 4E 53 4F 52 5F 41

This corresponds to:
Message type (u8) = 0x01
Sequence number (u16) = 42
Timestamp (u32) = 100000
Temperature (float) = 25.5
Device ID (string) = "SENSOR_A"
```

---

## Building
```
make
./packet_builder_demo
```

---

## Next Steps
This packet builder will serve as the foundation for upcoming projects:

- Binary Telemetry Recorder (write packet streams to disk)
 - Packet Parser + Validator (decode and validate recorded packets)
- Telemetry Simulator (generate packets periodically with logging)
- Ultimately, these components will be combined into a full telemetry packet system
capstone project suitable for a resume-ready GitHub portfolio.