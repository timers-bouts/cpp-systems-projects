# Project 4 — Binary Telemetry Recorder

This project builds a reusable **binary telemetry recording module** that can persist serialized telemetry packets to disk in a safe, structured, systems-style format.

It is the next step in my modern C++20 systems/embedded portfolio series, building toward a full telemetry packet capstone system.

This module forms the persistence layer for a complete telemetry system:
- Project 3: Packet serialization (PacketWriter)
- Project 4: Binary stream recording (TelemetryRecorder)
- Project 5 (next): Packet parsing + validation
- Final capstone: Full telemetry replay + checksum + CLI tools

---

## Goals

The `TelemetryRecorder` module supports:

- Writing raw telemetry packet byte buffers to a binary file
- Appending multiple packets over time (stream recording)
- A simple framed record format for safe parsing
- RAII-based file ownership (open in constructor, close automatically)
- Integration with the existing `PacketWriter` serializer library

---

## Shared Library Structure

The telemetry components are implemented as a reusable shared module:

```text
telemetry_lib/
include/telemetry/
PacketWriter.h
TelemetryRecorder.h
src/
PacketWriter.cpp
TelemetryRecorder.cpp
```

Project folders such as `04_recorder/` act as demo/test applications that build against this shared library.

---

## Binary File Format

The recorder writes telemetry streams using a simple framed format:

---

### File Header (8 bytes)

Written once at the beginning of a new file:

| Field   | Size | Description |
|---------|------|-------------|
| Magic   | 4    | ASCII `"TLRY"` |
| Version | 2    | Format version (`1`) |
| Flags   | 2    | Reserved (`0`) |

---

### Packet Records (Repeated)

Each packet is written as:

```text
[u32 packet_size][packet_bytes...]
```


Where:

- `packet_size` is a 32-bit little-endian length prefix
- `packet_bytes` is the raw serialized telemetry payload

Example:

```text

03 00 00 00 AA BB CC
05 00 00 00 12 34 56 78 9A
```

---

## Key Design Features

### RAII File Ownership

The recorder opens its output stream in the constructor and closes automatically when destroyed:

- No manual close required
- Safe cleanup even during exceptions

---

### Binary-Safe Output

Packets are written using `std::ofstream::write()` in binary mode:

- No formatting or text conversion
- Exact byte-for-byte persistence

---

### Stream Recording Support

The recorder supports two modes:

- `Truncate` — overwrite existing file
- `Append` — continue writing packets to the end of an existing stream

---

### Defensive Size Handling

Packets larger than `uint32_t` are rejected to prevent corrupted framing:

- Size prefix is fixed-width for future parsing

---

## Building and Running

From the `04_recorder/` directory:

```bash
make clean
make
./recorder_demo

This generates a binary output file such as:

```text
test.bin
```

Inspecting Output
To view the raw bytes written to disk:

```text
xxd -g 1 test.bin
```

Example output:

```text
54 4c 52 59 01 00 00 00   TLRY header
03 00 00 00 aa bb cc       packet 1
05 00 00 00 12 34 56 78 9a packet 2
```

---

## Why This Matters
This module forms the persistence layer for a complete telemetry system:
Project 3: Packet serialization (PacketWriter)
Project 4: Binary stream recording (TelemetryRecorder)
Project 5 (next): Packet parsing + validation
Final capstone: Full telemetry replay + checksum + CLI tools

---

## Next Steps
- Planned improvements in the next project:
- Reading packets back from disk safely
- Validating framing and header integrity
- Detecting corrupted or malformed streams
- Adding checksums for robustness




