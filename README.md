# C++ Systems Projects (Telemetry Prep Series)

This repository contains a sequence of small, focused C++ projects designed to
build practical systems-programming skills in modern C++.

The goal of this series is to develop strong foundations in:

- Modern C++ syntax and idioms (C++20)
- Object-oriented design
- RAII and resource management
- Binary-safe data handling
- File I/O and serialization
- Modular multi-file project structure

These projects serve as stepping stones toward a larger capstone project:

> **Telemetry Packet Builder + Recorder + Parser (Resume Project)**

---

## Repository Structure

```text
cpp-systems-projects/
  telemetry_lib/        Shared reusable telemetry modules
  01_converter/         CLI warmup
  02_logger/            Reusable logging utility
  03_packet_builder/    PacketWriter demo app
  04_recorder/          Binary telemetry recorder demo app
  05_packet_parser/         (upcoming)
  06_telemetry_simulator/   (upcoming)

```

PacketWriter and TelemetryRecorder now live in telemetry_lib/ and are reused across projects.

---

## Toolchain

```markdown
- `clang++` (C++20)
- GNU Make
- Visual Studio Code
```

---

## Completed Projects

### ✅ 01 — Unit Converter (CLI Warmup)

A simple command-line program to practice:

- C++ input/output (`std::cin`, `std::cout`)
- Functions and control flow
- Basic project structure

---

### ✅ 02 — Logger Utility

A reusable logging class built in modern C++.

Features include:

- Console + file output
- Timestamped log messages
- Log severity levels (`INFO`, `WARN`, `ERROR`)
- Minimum log level filtering
- RAII-based file management
- Modular header/source split (`Logger.h` / `Logger.cpp`)
- Makefile-based build workflow

Example output:

```text
2026-02-05 14:03:22 [INFO] Telemetry system started
2026-02-05 14:03:25 [WARN] Packet delay detected
2026-02-05 14:03:30 [ERROR] Sensor disconnected
```

### ✅ 03 — Telemetry Packet Builder

A reusable binary packet construction library built in modern C++20.

Features include:
- Safe byte-buffer packet building using std::vector<uint8_t>
- Type-safe field append methods (u8, u16, u32, float)
- Explicit endianness handling (big vs little endian)
- Raw payload support with add_bytes()
- Binary-safe export using std::span<const uint8_t>
- Efficient buffer reuse with reserve()
- Modular header/source split (PacketWriter.h / PacketWriter.cpp)
- Makefile-based build workflow

Example packet output (hex):
```
01 00 2A 00 01 86 A0 41 CC 00 00 00 08 53 45 4E 53 4F 52 5F 41
```

---

## Upcoming Projects

### 🔜 04 — Binary Telemetry Recorder

- Writing packet streams to disk
- File format design

### 🔜 05 — Packet Parser + Validator

- Reading binary telemetry logs
- Field decoding + checksum validation

### 🔜 06 — Telemetry Simulator (Capstone)

A full mini telemetry system:

- Packet generation
- Serialization/deserialization
- Logging + recording
- Resume-ready final deliverable

---

## Build Instructions

Each project contains its own Makefile or build commands.
Projects compile against sources in telemetry_lib/

Typical build workflow:

```bash
make
./program_name
make clean
```

---

## Purpose

This repository is part of a deliberate portfolio progression toward embedded-style
telemetry and binary protocol development.