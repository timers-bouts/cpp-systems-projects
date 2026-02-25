# Project 6 — Binary PacketReader (Endian-Aware Deserialization Core)

This project builds a reusable **binary packet parsing module** that safely reads structured data from raw byte buffers.

It is the next step in my modern C++20 systems/embedded portfolio series, continuing toward a full telemetry packet capstone system.

This module forms the deserialization layer of a complete telemetry pipeline:

- Project 3: Packet serialization (`PacketWriter`)
- Project 4: Binary stream persistence (`TelemetryRecorder`)
- Project 5: Binary packet replay + validation (`TelemetryReader`)
- **Project 6: Endian-aware byte deserialization (`PacketReader`)**
- Project 7: Typed frame decoding (`TelemetryFrame`)
- Final capstone: Full telemetry replay + checksums + CLI tools

---

## Goals

The `PacketReader` module provides:

- Safe sequential reading from `std::span<const uint8_t>`
- Endianness-aware integer decoding
- IEEE-754 float reconstruction
- Bounds-checked parsing
- Defensive exception handling on buffer underflow
- Zero dynamic allocation
- Cursor-style consumption semantics

This module is designed to be:

- Lightweight
- Reusable
- Deterministic
- Embedded-friendly
- Exception-safe

---

## Core Design

`PacketReader` consumes a byte span and advances a cursor as values are read:

```cpp
PacketReader reader(buffer, PacketReader::Endianness::Little);

uint64_t ts   = reader.read_u64();
float temp    = reader.read_float();
uint32_t flag = reader.read_u32();
```

Each read_* operation:

- Verifies sufficient remaining bytes
- Assembles the value according to configured endianness
- Advances the internal cursor
- Throws std::out_of_range if insufficient bytes remain

No partial reads are ever allowed.

---

## Supported Read Operations

The reader currently supports:

```text
PacketReader::Endianness::Little
PacketReader::Endianness::Big
```

All multi-byte reads respect the configured byte order.

This makes the module suitable for:

- Embedded firmware logs
- Network protocols
- Cross-platform binary formats
- File-based telemetry systems

---

## Float Reconstruction

Floating-point values are reconstructed from raw bytes using std::bit_cast:

```text
float value = std::bit_cast<float>(read_u32());
```

This provides:

- Strict-aliasing safety
- Clean intent expression
- Standards-compliant type punning
- Zero undefined behavior

The implementation assumes IEEE-754 32-bit floats, which is standard across modern platforms.

---

## Cursor Semantics

The reader operates as a forward-only cursor:

```text
|----bytes----|
^
cursor starts here
```

After each read:

```text
|----bytes----|
        ^
        cursor advances
```

Once all bytes are consumed:

```text
reader.empty() == true
```

This design ensures:

- No manual index tracking
- No mutable offsets exposed
- No accidental double-reads
- Clean sequential parsing logic

---

## Defensive Parsing

The module prevents buffer overrun by checking bounds before every read.

Example protection:

```text
if (dst.size() > remaining())
    throw std::out_of_range(...);
```

This guarantees:

- No undefined behavior
- No memory corruption
- Immediate failure on malformed input

This is critical for:

- Embedded systems
- Network protocol parsers
- Safety-critical telemetry systems

---

## Library Structure

PacketReader is part of the shared telemetry library:

```text
telemetry_lib/
  include/telemetry/
    PacketWriter.h
    PacketReader.h
  src/
    PacketWriter.cpp
    PacketReader.cpp
```

This project contains the core parsing logic only.

Testing and frame decoding are implemented in later projects.

---

## Why This Matters

Binary deserialization is a foundational systems programming skill.

This module demonstrates:

- Careful memory handling
- Defensive boundary checking
- Endianness correctness
- Clean C++20 usage (std::span, std::bit_cast)
- Separation of concerns
- Reusable architecture

This is directly applicable to:

- Embedded device logs
- Flight telemetry
- Automotive event recorders
- Robotics data capture
- Binary network protocols
- High-performance systems software

---

## Next Steps

Planned continuation in upcoming projects:

Project 7: TelemetryFrame serializer/deserializer

- Round-trip validation testing
- Typed message decoding
- Message IDs + versioning
- Checksum support
- CLI-based replay tools
- Full capstone telemetry pipeline

This project completes the core binary parsing layer of the telemetry system.
It pairs directly with PacketWriter to form a full serialization/deserialization foundation.