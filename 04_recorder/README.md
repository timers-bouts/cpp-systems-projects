# Binary Telemetry Recorder (C++20)

A reusable **binary telemetry recording module** that persists serialized telemetry packets to disk using a framed, CRC-protected format.

This module is part of a larger modern C++ systems/embedded telemetry architecture, serving as the persistence layer between packet serialization and packet parsing.

---

# Overview

`TelemetryRecorder` provides:

- Binary-safe packet persistence
- Framed record structure for safe replay
- CRC32 integrity protection
- RAII-based file ownership
- Stream append or truncate modes
- Structured logging integration
- Move-only resource semantics

This module is designed in a systems-programming style suitable for embedded logging, flight software, or data acquisition pipelines.

---

# Library Structure

## Shared Library Structure

The telemetry components are implemented as a reusable shared module:

```text
└── telemetry_lib
    ├── include
    │   └── telemetry
    │       ├── CRC.h
    │       ├── Logger.h
    │       ├── PacketReader.h
    │       ├── PacketWriter.h
    │       ├── TelemetryFormat.h
    │       ├── TelemetryReader.h
    │       └── TelemetryRecorder.h
    └── src
        ├── CRC.cpp
        ├── Logger.cpp
        ├── PacketReader.cpp
        ├── PacketWriter.cpp
        ├── TelemetryReader.cpp
        └── TelemetryRecorder.cpp
```


Demo/test applications (e.g., `04_recorder/`) link against this shared module.

---

# Binary File Format

## File Header (8 bytes)

Written once when creating a new file:

| Field   | Size | Description |
|----------|------|------------|
| Magic    | 4    | ASCII `"TLRY"` |
| Version  | 2    | Format version (`1`) |
| Flags    | 2    | Reserved (`0`) |

---

## Packet Record Format

Each packet is written as:

```text
[u32 size_le][payload bytes...][u32 crc32_le]
```

Where:

- `size_le` is a 32-bit little-endian length
- The size includes `payload + CRC`
- `crc32_le` is the CRC-32 checksum of the payload only

Example layout:

```text
54 4C 52 59 01 00 00 00 TLRY header
0B 00 00 00 size = 11 bytes
AA BB CC DD EE payload (5 bytes)
12 34 56 78 crc32
```

---


This framing allows:

- Safe sequential parsing
- Corruption detection
- Stream validation

---

# Key Design Features

## RAII File Ownership

- File opened in constructor
- Automatically closed on destruction
- No manual resource management required

---

## CRC32 Integrity Protection

Each packet includes a CRC-32 checksum:

- Detects corrupted payloads
- Enables safe replay validation
- Provides forward compatibility for parsing tools

---

## Binary-Safe Output

All writes use:

```text
std::ofstream::write()
```

with `std::ios::binary`, ensuring:

- No text formatting
- Exact byte preservation
- Cross-platform deterministic layout

---

## Stream Recording Modes

Two open modes are supported:

- `Truncate` — overwrite existing file
- `Append` — append to existing telemetry stream

Header is written only when:

- Creating a new file
- Appending to an empty file

---

## Defensive Error Handling

The output stream enables exception flags:

```text
out_.exceptions(std::ios::failbit | std::ios::badbit);
```

All write operations:

- Detect I/O failures
- Throw meaningful `std::runtime_error`
- Prevent silent corruption

---

## Structured Logging Integration

The recorder integrates with the project’s `Logger` system:

- Logs file open events
- Logs header creation
- Logs packet writes (size + CRC)
- Logs error conditions

This enables observability during simulation and replay testing.

---

## Move-Only Semantics

`TelemetryRecorder` is:

- Non-copyable
- Move-constructible
- Not move-assignable

This prevents accidental duplication of file handles and enforces correct ownership semantics.

---

# Building

From the demo directory:

```bash
make clean
make
./recorder_demo

This produces a binary telemetry file (e.g., test.bin).

Inspecting Output
To view the raw bytes written to disk:

```text
xxd -g 1 test.bin
```

Example output:

```text
54 4c 52 59 01 00 00 00
0b 00 00 00 aa bb cc dd ee 12 34 56 78
```

---

## Role in the Larger Telemetry Systems

This module provides the persistence layer in a staged telemetry architecture:

- Project 2: Logger
- Project 3: Packet serialization (PacketWriter)
- **Project 4: Binary stream recording (TelemetryRecorder)**
- Project 5: Packet parsing + validation
- Project 6: Packet deserialization (PacketReader)
- Project 7: Mini simulator test (in preparation for capstone)
- Final capstone: Full telemetry replay + checksum + CLI tools

---

## Why This Matters

This module demonstrates:

- Systems-style binary file design
- Framed streaming protocols
- CRC-based integrity protection
- RAII resource ownership
- Exception-safe I/O
- Move semantics in C++20

It is designed to resemble real-world embedded or flight-data logging systems.

---

## Next Steps
- Planned improvements in the next project:
- Reading packets back from disk safely
- Validating framing and header integrity
- Detecting corrupted or malformed streams
