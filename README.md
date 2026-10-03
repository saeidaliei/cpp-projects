# Modern C++ Practice Projects

A hands-on C++23 practice path containing 15 small projects. The projects are deliberately independent: they can be built and studied one at a time, then return later and refactor them as you learn more.

## What this repo is for

The goal is not to memorize syntax. Each project gives a small problem where a particular part of modern C++ is useful. There are also exercises from the README of each project.

## Project map

| # | Project | Main topics |
|---|---|---|
| 01 | Task Manager CLI | classes, RAII, `std::optional`, `std::filesystem`, file I/O |
| 02 | Text Analyzer | ranges, views, lambdas, algorithms, `string_view` |
| 03 | CSV Expense Analyzer | `std::expected`, `std::from_chars`, parsing, error handling |
| 04 | Geometry & Concepts | concepts, generic programming, operators, `constexpr` |
| 05 | LRU Cache | templates, iterators, list/hash-map composition |
| 06 | File Indexer | recursive filesystem traversal, aggregation, clean APIs |
| 07 | Event Bus | type erasure, `std::any`, `std::function`, `std::type_index` |
| 08 | Expression Evaluator | recursive descent parsing, `std::variant`, visitors |
| 09 | Thread Pool | `std::jthread`, mutexes, condition variables, futures |
| 10 | Producer/Consumer Pipeline | bounded queues, back-pressure, cooperative shutdown |
| 11 | Coroutine Generator | C++20/23 coroutines, promise types, iterators |
| 12 | Graph Toolkit | graph representations, BFS, Dijkstra, generic algorithms |
| 13 | PMR Memory Lab | `<memory_resource>`, custom resources, allocation behavior |
| 14 | SPSC Ring Buffer | atomics, acquire/release ordering, lock-free design |
| 15 | Type-Safe Units | `std::ratio`, strong types, `consteval`, user-defined literals |

## Toolchain

The repository asks CMake for C++23 (`cxx_std_23`). It is intentionally standard-library-only so the focus can be on the language and library instead of dependency management.

A recent GCC or Clang plus CMake should work. Some implementation details, especially around coroutines and synchronization primitives, may differ slightly between standard-library implementations.

## Build everything

```bash
cmake -S . -B build
cmake --build build -j
```

Executables are placed under `build/bin/`.

Run one project, for example:

```bash
./build/bin/cpp04_geometry_concepts
```

## Build one project

You can also configure the repo once and build a single target:

```bash
cmake --build build --target cpp09_thread_pool
```

## Suggested learning order

The numerical order is intentional. Projects 1-5 establish common library and generic-programming patterns. Projects 6-8 move into systems-style APIs and parsing. Projects 9-14 are concurrency/low-level topics. Project 15 closes with compile-time type design.

## Practice philosophy

The source is intentionally commented, but comments explain *why* a construct exists rather than translating every line into English.

Each project README contains a `Practice tasks` section. The tasks start small and become increasingly open-ended.

## Repository layout

```text
modern_cpp_projects/
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── .gitignore
└── projects/
    ├── 01_task_manager/
    ├── 02_text_analyzer/
    ├── ...
    └── 15_type_safe_units/
```
