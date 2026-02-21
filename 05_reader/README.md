# Project 5 — Binary Telemetry Reader + Defensive Packet Parser

This project builds a reusable **binary telemetry reading and validation module** that can safely parse recorded telemetry streams back from disk.

It is the next step in my modern C++20 systems/embedded portfolio series, continuing toward a full telemetry packet capstone system.

This module forms the replay + validation layer of a complete telemetry pipeline:

- Project 3: Packet serialization (`PacketWriter`)
- Project 4: Binary stream persistence (`TelemetryRecorder`)
- **Project 5: Binary packet parsing + validation (`TelemetryReader`)**
- Final capstone: Full telemetry replay + checksums + CLI tools

---

## Goals

The `TelemetryReader` module supports:

- Opening and validating telemetry `.bin` recording files
- Verifying file header integrity (magic + version)
- Reading framed packet records back safely:

```text
  [u32 packet_size][packet_bytes...]
```

- Defensive parsing against corrupted or malformed streams
- Preventing oversized allocations with configurable limits
- Returning packets as raw std::vector<uint8_t> payloads

## Shared Library Structure

The telemetry components are implemented as a reusable shared module:

```text
telemetry_lib/
include/telemetry/
PacketWriter.h
TelemetryRecorder.h
TelemetryReader.h
TelemetryFormat.h
src/
PacketWriter.cpp
TelemetryRecorder.cpp
TelemetryReader.cpp
```

Project folders such as 05_reader/ act as demo/test applications that build against this shared library.

## Binary File Format

The reader parses the same framed telemetry format written by Project 4:

File Header (8 bytes)
Validated once at file open:
Field	Size	Description
Magic	4	ASCII "TLRY"
Version	2	Format version (1)
Flags	2	Reserved (0)

If the header is invalid or truncated, the reader throws a ParseError.

### Packet Records (Repeated)

Each telemetry record is stored as:
```text
[u32 packet_size][packet_bytes...]
```

Where:

- packet_size is a 32-bit little-endian length prefix
- packet_bytes is the raw serialized telemetry payload

Example stream:
```text
03 00 00 00 AA BB CC
05 00 00 00 12 34 56 78 9A
```

## Key Design Features

### Defensive Parsing + Validation

The reader is designed to safely reject malformed telemetry streams:

- Detects truncated headers
- Rejects incomplete size fields
- Detects truncated payload records
- Rejects invalid packet sizes (0 or oversized)
- All corruption triggers a ParseError exception.

### Clean EOF Semantics

Packet reading uses an iterator-style API:
```text
bool read_next(std::vector<uint8_t>& out_packet);
```

Behavior:

- Returns true when a full packet is successfully read
- Returns false only on clean EOF at a record boundary
- Throws ParseError on corruption or truncation

### Oversized Allocation Protection

To prevent malicious or corrupted size fields from forcing huge allocations, the reader enforces a configurable maximum:
```text
TelemetryReader::Options opt;
opt.max_packet_size = 64 * 1024;
```

Any record larger than this limit is rejected immediately.

### Exact Binary Reads

The reader uses a defensive internal helper:

- read_exact()

Which guarantees:

- Either the requested bytes are fully read
- Or truncation/corruption is detected immediately

This prevents partial reads or silent stream failures.

## Building and Running

From the 05_reader/ directory:

```text
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

## Inspecting Telemetry Recordings

To view the raw bytes on disk:

```text
xxd -g 1 test.bin
```

Example:

```text
54 4c 52 59 01 00 00 00   TLRY header
03 00 00 00 aa bb cc       packet 1
05 00 00 00 12 34 56 78 9a packet 2
```

## Why This Matters

This module completes the core telemetry loop:

- Project 3 builds packets
- Project 4 records packets to disk
- Project 5 replays packets safely back from disk

Defensive binary parsing is a critical systems skill in:

- embedded logging
- flight telemetry
- automotive event recording
- robotics trace capture
- safety-critical replay pipelines

## Next Steps

Planned improvements in upcoming projects:

- Project 6: Telemetry Simulator (device-style packet producer)
- Project 7: Full serializer/deserializer system
- Message IDs + typed decoding
- Checksums + validation
- Recording + replay CLI tools
- Tests + documentation
- Professional GitHub-ready capstone