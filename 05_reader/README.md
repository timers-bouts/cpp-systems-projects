# Binary Telemetry Reader (C++20)

A reusable **binary telemetry reading and validation module** that safely parses recorded telemetry streams from disk.

This module completes the replay + validation layer of a modern C++ telemetry infrastructure system.

It is designed with defensive parsing, corruption detection, and strict binary validation suitable for embedded logging and safety-critical replay pipelines.

---

# Overview

`TelemetryReader` provides:

- Binary-safe telemetry file parsing
- Header validation (magic + version + flags)
- Framed packet decoding
- CRC32 payload verification
- Defensive corruption detection
- Oversized allocation protection
- Clean EOF semantics
- Structured logging integration
- Move-only resource semantics

This module is built to resemble real-world telemetry replay systems used in embedded, robotics, and flight-style logging environments.

---

# Library Structure

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


Demo/test applications such as `05_reader/` build against this shared module.

---

# Binary File Format

The reader parses the same framed format written by `TelemetryRecorder`.

---

## File Header (8 bytes)

Validated once at file open:

| Field   | Size | Description |
|----------|------|------------|
| Magic    | 4    | ASCII `"TLRY"` |
| Version  | 2    | Format version (`1`) |
| Flags    | 2    | Reserved (`0`) |

If:

- Magic does not match
- Version is unsupported
- Header is truncated

The reader throws a `ParseError`.

---

## Packet Record Format

Each telemetry record is stored as:

[u32 size_le][payload bytes...][u32 crc32_le]

Where:

- `size_le` is a 32-bit little-endian length
- Size includes `payload + CRC`
- `crc32_le` is the CRC-32 of the payload only

Example stream:

```text
54 4C 52 59 01 00 00 00 TLRY header
0B 00 00 00 size = 11 bytes
AA BB CC DD EE payload (5 bytes)
12 34 56 78 crc32
```

---

# Key Design Features

## Defensive Parsing + Validation

The reader is intentionally strict and rejects malformed streams:

- Detects truncated headers
- Rejects incomplete size fields
- Detects truncated payloads
- Validates CRC32 checksum
- Rejects zero-length or oversized packets
- Structured logging integration
- Throws `ParseError` on corruption

No silent failures.

---

## Clean EOF Semantics

Packet reading uses an iterator-style API:

```text
bool read_next(std::vector<uint8_t>& out_packet);
```

Behavior:

- Returns `true` when a full, valid packet is read
- Returns `false` only on clean EOF at a record boundary
- Throws `ParseError` on corruption or truncation

This clean separation simplifies replay loops.

---

## Oversized Allocation Protection

To prevent malicious or corrupted size fields from forcing huge allocations:

```text
TelemetryReader::Options opt;
opt.max_packet_size = 64 * 1024;
```

Any record exceeding this limit is rejected immediately.

This protects against memory exhaustion attacks and corrupted files.

---

## Exact Binary Reads

The reader uses a defensive internal helper (`read_exact`) that guarantees:

- Either the requested bytes are fully read
- Or truncation is detected immediately

This prevents partial reads and silent stream state failures.

---

## CRC32 Verification

Each packet’s CRC is recomputed and verified during parsing:

- Detects payload corruption
- Ensures integrity across write → disk → read cycle
- Enables safe replay validation

If CRC does not match, a `ParseError` is thrown.

---

## Structured Logging Integration

The reader integrates with the project’s `Logger` system:

- Logs file open + header validation
- Logs packet reads (size + CRC)
- Logs corruption events
- Logs clean EOF

This provides observability during simulation and replay testing.

---

## Move-Only Semantics

`TelemetryReader` is:

- Non-copyable
- Move-constructible
- Not move-assignable

This prevents accidental duplication of file streams and enforces proper ownership semantics.

---

# Building

From the `05_reader/` directory:

```bash
make clean
make
make run ARGS="test.bin"
```

Example output:

```text
Packet 1: 3 bytes
aa bb cc

Packet 2: 5 bytes
12 34 56 78 9a
```

---

## Inspecting Telemetry Recordings

To view the raw bytes on disk:

```text
xxd -g 1 test.bin
```

Example:

```text
54 4c 52 59 01 00 00 00
0b 00 00 00 aa bb cc dd ee 12 34 56 78
```

---

## Role in the Larger Telemetry Systems

This module provides the persistence layer in a staged telemetry architecture:

- Project 2: Logger
- Project 3: Packet serialization (PacketWriter)
- Project 4: Binary stream recording (TelemetryRecorder)
- **Project 5: Packet parsing + validation**
- Project 6: Packet deserialization (PacketReader)
- Project 7: Mini simulator test (in preparation for capstone)
- Final capstone: Full telemetry replay + checksum + CLI tools

---

## Why This Matters

This module demonstrates:

- Defensive binary protocol parsing
- Framed streaming format validation
- CRC-based integrity protection
- Exception-safe I/O
- RAII resource ownership
- Move semantics in C++20
- Memory-safety considerations in systems programming

These are core skills in:

- Embedded logging systems
- Robotics trace capture
- Automotive event recording
- Flight telemetry replay
- Safety-critical data pipelines

---

## Next Steps

Planned improvements in upcoming projects:

- Project 6: Packet Deserializer (Packet Reader)
- Project 7: Telemetry Simulator (device-style packet producer)
- Typed message decoding (Message IDs)
- Structured packet deserialization
- CLI replay + inspection tools
- JSON export mode
- Simulator integration
- Full capstone telemetry system