# Project 7 — Mini Telemetry Simulator

This project builds a **configurable telemetry frame generator** for modern C++20 systems, enabling end-to-end testing of binary serialization, recording, and deserialization pipelines.

It is the next step in my telemetry portfolio series, building on previous modules to provide **live frame generation and replay testing**.

This module forms the simulation layer of a complete telemetry pipeline:

- Project 3: Packet serialization (`PacketWriter`)  
- Project 4: Binary stream persistence (`TelemetryRecorder`)  
- Project 5: Binary packet replay + validation (`TelemetryReader`)  
- Project 6: Endian-aware byte deserialization (`PacketReader`)  
- **Project 7: Mini Telemetry Simulator (`TelemetrySimulator`)**  
- Final capstone: Full telemetry replay + checksums + CLI tools  

---

## Goals

The `TelemetrySimulator` module supports:

- Generating structured telemetry frames over time  
- Configurable start time, step size, and simulation duration  
- Realistic field evolution with drift, noise, and state changes  
- Deterministic output for reproducible simulations  
- Serialization of frames via `TelemetryFrame::serialize(PacketWriter&)`  
- Safe recording of frames using `TelemetryRecorder` with CRC32 validation  
- Optional in-memory frame storage for immediate access or testing  
- Integration with `PacketReader` and `TelemetryFrame::deserialize` for round-trip validation  

This module is designed to be:

- Lightweight and modular  
- Embedded-friendly  
- Testable and deterministic  
- Easy to extend with new telemetry channels  

---

## Core Design

The simulator operates as a **time-stepped frame generator**:

```cpp
TelemetrySimulator sim(config, recorder);
sim.run(); // generates frames, serializes, writes to recorder

Each iteration:

- Generates a TelemetryFrame reflecting the current simulated state
- Serializes the frame into a PacketWriter buffer
- Writes the serialized packet to a TelemetryRecorder
- Advances internal simulation state (position, velocity, temperature, voltage, etc.)
- Optionally, frames can be stored in memory for testing:

```cpp
auto frames = sim.get_frames();
for (const auto& f : frames)
    print_frame(f);
```

---

## Simulation Fields

Each TelemetryFrame includes:

- Field	Units
- Timestamp	milliseconds
- Temperature	degrees Celsius
- Voltage	Volts
- Position X/Y	Meters
- Velocity	Meters per second
- Status Flags	Bitmask / device status

Field values evolve realistically:

- Randomized noise added per step
- Linear or sinusoidal drift supported
- Optional configurable ranges for testing edge cases

---

## Deterministic Reproducibility

The simulator supports a fixed random seed:

- Ensures identical frame sequences across runs
- Facilitates unit testing and validation
- Allows regression testing with consistent telemetry patterns

---

## Integration with Deserialization

The simulator is fully compatible with your deserialization pipeline:

```cpp
TelemetrySimulator sim(config, recorder);
sim.run();

auto raw_packets = recorder.read_all();
for (auto& pkt : raw_packets) {
    PacketReader reader(pkt, PacketReader::Endianness::Big);
    TelemetryFrame frame;
    frame.deserialize(reader);
    print_frame(frame);
}
```

This provides end-to-end validation of:

- PacketWriter → PacketReader round-trips
- CRC32 validation in recorder
- Field correctness and simulation fidelity

---

## Library Structure

The simulator is implemented as part of the shared telemetry library:

```text
telemetry_lib/
  include/telemetry/
    TelemetrySimulator.h
    TelemetryFrame.h
  src/
    TelemetrySimulator.cpp
    TelemetryFrame.cpp
```

Simulation configuration is kept separate from the core frame and recorder modules for modularity.

---

## Why This Matters

The mini simulator completes the testable telemetry loop:

- Generates realistic telemetry data
- Integrates with serialization and deserialization modules
- Supports deterministic, reproducible testing
- Provides a foundation for building a full replay, monitoring, and analysis pipeline

It demonstrates:

- Modern C++20 practices (std::span, exception safety, modular design)
- Clean separation of simulation, serialization, and recording logic
- Systems-level understanding of binary telemetry pipelines
- A professional, end-to-end portfolio-ready example

---

## Next Steps

The mini simulator sets the stage for:

- Full telemetry replay and CLI-based visualization
- Message IDs, versioning, and typed decoding
- End-to-end capstone with CRC validation and real-time monitoring
- Optional CSV/JSON export and plotting tools
- Integration with unit tests and GitHub-ready examples

This project provides a robust, reusable telemetry generator for validating any downstream packet processing and deserialization modules.