# High-Throughput VWAP Calculation Engine

A low-latency C++17 market data parser designed to aggregate Volume Weighted Average Price (VWAP) across massive historical tick datasets.

## Overview
Parsing large CSV files (millions of rows) using standard C++ streams (`std::stringstream`) or Python often bottlenecks on memory allocation and string copying. This engine is built to process millions of rows of comma-separated market data in fractions of a second by eliminating heap allocations during the critical parsing phase.

## Core Engineering Focus
* **Zero-Copy Tokenization:** Utilizes C++17 `std::string_view` to slice incoming text streams without copying string buffers or triggering `malloc`/`new`.
* **Bare-Metal Parsing:** Bypasses locale-dependent standard library functions (`std::stod`, `std::stoi`) in favor of `<charconv>` (`std::from_chars`), executing string-to-numeric conversions directly on raw memory pointers.
* **$O(1)$ State Aggregation:** Maintains running mathematical totals for unique ticker symbols simultaneously using `std::unordered_map`, keeping the memory footprint flat regardless of the dataset size.

## Performance Benchmarks
## Performance Benchmarks
Tested on a standard Windows laptop compiling with GCC (`-O3` optimization). 
Successfully parsed and aggregated **5,000,000 rows** (~150MB CSV).

| Metric | Result |
| :--- | :--- |
| **Total Rows Processed** | 5,000,000 |
| **Wall-Clock Time** | ~1.15s |
| **Throughput** | **~4.3M rows / second** |
| **Peak Memory Footprint** | < 15 MB |

Note: The VWAP calculation dynamically updates in $O(1)$ time per tick without needing to store individual historical trades in memory.

## Project Structure
```text
FastVWAP_Engine/
├── CMakeLists.txt         # Build configuration
├── README.md
└── src/
    ├── Types.h            # Core data structures (VWAPMetric)
    ├── VWAPEngine.h       # Engine definitions
    ├── VWAPEngine.cpp     # Zero-copy parsing and aggregation logic
    ├── DataGenerator.cpp  # Utility to generate millions of mock CSV rows
    └── main.cpp           # Execution and std::chrono benchmarking
