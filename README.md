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

### ✅ 04 — Binary Telemetry Recorder
A reusable binary telemetry recording module built in modern C++20.

Features include:
- Stream-based telemetry packet persistence using std::ofstream in binary mode
- Safe framed recording format: [u32 packet_size][packet_bytes...]
- Support for writing multiple packets over time (continuous append recording)
- RAII-based file ownership (open in constructor, close automatically)
- Optional append vs truncate modes for flexible recording workflows
- Fixed file header with magic signature ("TLRY") and versioning support
- Length-prefix framing for future parsing and validation (Project 5)
- Integration with PacketWriter via std::span<const uint8_t> zero-copy packet export
- Modular reusable library design inside telemetry_lib/
- Makefile-based build + demo testing workflow

Example recorded stream output (hex):
```
54 4C 52 59 01 00 00 00
03 00 00 00 AA BB CC
05 00 00 00 12 34 56 78 9A
```

### ✅ 05 — Binary Telemetry Reader + Defensive Parser
A reusable binary telemetry reading and validation module built in modern C++20.

Features include:
- Stream-based telemetry replay using std::ifstream in binary mode
- Safe framed parsing of format: [u32 packet_size][packet_bytes...]
- Header validation with fixed magic signature ("TLRY") and version enforcement
- Strict truncation detection for header, size fields, and payload data
- Clean EOF semantics via iterator-style read_next() API
- Configurable maximum packet size to prevent oversized memory allocations
- Custom ParseError exception for structured error handling
- Defensive binary read helper (read_exact) to prevent partial-read corruption
- Full manual test harness covering valid and malformed file cases
- Modular reusable library design inside telemetry_lib/
- Makefile-based build + standalone test execution workflow

Example validated stream input (hex):
```
54 4C 52 59 01 00 00 00
03 00 00 00 AA BB CC
05 00 00 00 12 34 56 78 9A
```

---

## Upcoming Projects

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