# 11 - Coroutine Generator

Build a small lazy generator abstraction with C++20 coroutines.

## Concepts

- `co_yield`
- `std::coroutine_handle`
- promise types
- lazy execution
- custom iterators
- lifetime management

## Key idea

Calling a coroutine does not run it to completion. It creates a coroutine frame. `co_yield` suspends execution and lets the consumer pull values one at a time.

## Practice tasks

1. Make the generator work with move-only values.
2. Add `begin()`/`end()` support to generate infinite sequences safely.
3. Implement a `filter()` coroutine that accepts another generator.
4. Generate lines from a file lazily.
5. Add `co_return` of a final result and expose it after iteration.
