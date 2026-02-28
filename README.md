# C++ Systems Projects — Telemetry Prep Series

This repository contains a sequence of small, focused C++ projects designed to
build **practical systems-programming skills** in modern C++ (C++20).

The series demonstrates:

- Modern C++ syntax and idioms
- Object-oriented design and encapsulation
- RAII and resource management
- Binary-safe data handling and serialization
- File I/O and structured streaming
- Modular, reusable multi-file project structure
- Defensive programming for embedded-style telemetry pipelines

The ultimate goal of this portfolio is a **resume-ready telemetry capstone project**:

> **Telemetry Packet Builder + Recorder + Parser + Simulator**

---

## Repository Structure

```text
cpp-systems-projects/
├── telemetry_lib/ Shared reusable telemetry modules
│ ├── include/telemetry/
│ │ ├── PacketWriter.h
│ │ ├── TelemetryRecorder.h
│ │ ├── TelemetryReader.h
│ │ ├── PacketReader.h
│ │ ├── TelemetryFormat.h
│ │ ├── CRC.h
│ │ └── Logger.h
│ └── src/
│ ├── PacketWriter.cpp
│ ├── TelemetryRecorder.cpp
│ ├── TelemetryReader.cpp
│ ├── PacketReader.cpp
│ └── Logger.cpp
├── 01_converter/ CLI warmup
├── 02_logger/ Logging utility demo
├── 03_packet_writer/ PacketWriter demo app
├── 04_recorder/ Binary telemetry recorder demo app
├── 05_reader/ Binary telemetry reader demo app
├── 06_packet_reader/ PacketReader deserialization helper
└── 07_telemetry_simulator/ Mini telemetry simulator
```

---

## Toolchain

- `clang++` (C++20)
- GNU Make
- Visual Studio Code (optional)

---

## Completed Projects

### ✅ 01 — Unit Converter (CLI Warmup)

- Practice modern C++ input/output (`std::cin`, `std::cout`)
- Functions, control flow, and simple project structure

---

### ✅ 02 — Logger Utility

- Reusable logging class (`Logger`) with:
  - File + console output
  - Timestamped messages
  - Severity levels (`INFO`, `WARN`, `ERROR`)
  - Minimum log-level filtering
  - RAII-based file management

Example:

```text
2026-02-05 14:03:22 [INFO] Telemetry system started
2026-02-05 14:03:25 [WARN] Packet delay detected
2026-02-05 14:03:30 [ERROR] Sensor disconnected
```

---

### ✅ 03 — Telemetry Packet Builder

- Safe, type-aware binary packet construction
- Endianness-aware encoding (big vs little)
- Payload support via `add_bytes()`
- Efficient buffer reuse with `reserve()`
- Zero-copy export with `std::span<const uint8_t>`

Example hex output:

```text
01 00 2A 00 01 86 A0 41 CC 00 00 00 08 53 45 4E 53 4F 52 5F 41
```

---

### ✅ 04 — Binary Telemetry Recorder

- Binary-safe stream recording (`std::ofstream`)
- Framed packet format with CRC32: `[u32 size][payload][u32 crc]`
- Fixed header with magic `"TLRY"` and versioning
- RAII file ownership
- Truncate vs append modes
- Integration with `PacketWriter`

Example recorded stream:

```text
54 4C 52 59 01 00 00 00
03 00 00 00 AA BB CC
05 00 00 00 12 34 56 78 9A
```

---

### ✅ 05 — Binary Telemetry Reader + Defensive Parser

- Binary-safe stream replay (`std::ifstream`)
- Framed parsing with CRC32 validation
- Header validation: magic, version, flags
- Strict truncation detection
- Iterator-style `read_next()` API
- Configurable `max_packet_size` to prevent large allocations
- ParseError exception for corrupted streams
- Full logging support

Example input:

```text
54 4C 52 59 01 00 00 00
03 00 00 00 AA BB CC
05 00 00 00 12 34 56 78 9A
```

---

### ✅ 06 — PacketReader: Binary Telemetry Deserialization

- Sequential reading from `std::span<const uint8_t>`
- Read operations: u8, u16, u32, u64, float, arbitrary spans
- Configurable endianness
- Bounds-checked reads with exceptions
- Cursor-style semantics (forward-only)
- Integration with `TelemetryFrame::deserialize()`

Example usage:

```cpp
PacketReader reader(pw.bytes(), PacketReader::Endianness::Big);
TelemetryFrame frame;
frame.deserialize(reader);
print_frame(frame);
```

---

### ✅ 07 — Mini Telemetry Simulator

- Generates realistic telemetry frames:
- Timestamp, position, velocity, temperature, voltage, status flags
- Configurable step size, duration, and noise
- Serializes frames via TelemetryFrame::serialize(PacketWriter&)
- Writes to TelemetryRecorder with CRC validation
- Optional in-memory frame access for testing
- Designed to integrate with PacketReader for replay

Examle simulation loop:

```cpp
TelemetrySimulator sim(config, recorder);
sim.run();             // generate + serialize + record frames
auto frames = sim.get_frames(); // optional in-memory access
```

---

## Upcoming Capstone

A final, resume-ready project:

- Full telemetry system:
  - Packet generation
  - Serialization/deserialization
  - Logging + recording + replay
  - CLI tools + optional CSV/JSON export
  - Replay validation with CRCs
- Designed to showcase production-quality C++ systems engineering skills

---

## Build Instructions

All projects compile against sources in telemetry_lib/.

Typical workflow:

```text
make
./program_name
make clean
```

---

## Purpose

This repository demonstrates a progressive, portfolio-focused path toward:

- Embedded telemetry pipelines
- Safe binary protocol design
- Modular, reusable C++20 systems code
- Defensive programming for file I/O and serialization
- Resume-ready capstone projects