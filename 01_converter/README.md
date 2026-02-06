# Unit Converter (C++ CLI)

This project is a simple command-line unit converter written in modern C++.

It was built as the first warm-up project in a larger sequence of C++ systems
projects leading toward a telemetry packet serialization and simulation
capstone.

---

## Features

- Converts between common units, such as:
  - Celsius → Fahrenheit
  - Meters → Feet
  - Bytes → Kilobytes

- Simple interactive command-line interface

---

## Purpose

This project was designed to practice foundational C++ concepts, including:

- Basic program structure (`main`)
- Console input/output (`std::cin`, `std::cout`)
- Functions and control flow
- Compiling and running C++ programs with `clang++`

---

## Example Usage

=== Unit Converter ===
1. Celsius -> Fahrenheit
2. Meters -> Feet
3. Bytes -> Kilobytes

Choose an option: 1

Enter value: 25
Result: 77 F


---

## Building

Compile using `clang++` with C++20:

```bash
clang++ -std=c++20 converter.cpp -o converter
```
