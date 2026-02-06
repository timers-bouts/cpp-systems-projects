# Logger Utility (C++)

This project is a simple reusable logging utility written in modern C++20.

It was built as part of a sequence of smaller C++ systems projects designed to
develop foundational skills before building a larger telemetry packet simulator.

The logger supports:

- Writing log messages to a file
- Printing log messages to the console
- Log severity levels (`INFO`, `WARN`, `ERROR`)
- Timestamped output
- Minimum log level filtering (e.g., log only WARN+)

---

## Project Structure

```text
02_logger/
  include/Logger.h
  src/Logger.cpp
  main.cpp
  Makefile
  ```

  ---

## Features

- **Modern C++ class design**
- RAII-based file management (automatic cleanup)
- Enum-based log levels with severity filtering
- Modular structure using separate header/source files

Example log output:

```text
2026-02-05 14:03:22 [INFO] Telemetry system started
2026-02-05 14:03:25 [WARN] Packet delay detected
2026-02-05 14:03:30 [ERROR] Sensor disconnected
```

---

## Building

```bash
make
./logger
```

