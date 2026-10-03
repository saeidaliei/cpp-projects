# 01 - Task Manager CLI

A tiny command-line todo manager that persists tasks to a text file. It is intentionally small enough to understand in one sitting.

## Concepts

- classes and invariants
- `enum class`
- RAII and file streams
- `std::optional`
- `std::filesystem`
- `std::chrono` for IDs/timestamps
- command parsing with `std::getline`

## Run

```bash
./build/bin/cpp01_task_manager
```

The program stores data in `tasks.txt` in the current working directory.

## What to notice

`TaskStore` owns the persistence detail. `main()` deals with user interaction. That separation is the first small step toward testable code.

The code deliberately uses value-like `Task` objects and a `std::vector` rather than pointers. Start by asking yourself why that makes ownership easier to reason about.

## Practice tasks

1. Add a priority field (`low`, `medium`, `high`) and persist it.
2. Add a `due` date using `std::chrono::sys_days`.
3. Add a `find <keyword>` command.
4. Replace the ad-hoc text format with a versioned format of your own.
5. Extract the command parser into a separate class and write tests for it.
