# 14 - SPSC Ring Buffer

A single-producer / single-consumer bounded queue implemented with atomics instead of a mutex.

## Concepts

- `std::atomic`
- acquire/release memory ordering
- cache-friendly fixed-size storage
- lock-free data structures
- single-producer/single-consumer ownership assumptions

## Important warning

This data structure is only correct for one producer and one consumer. Adding a second producer or consumer changes the synchronization problem completely.

The easiest way to understand the memory ordering is to ask: *when the consumer observes the published write index, what earlier writes must also be visible?*

## Practice tasks

1. Add `try_push` for move-only objects.
2. Add a non-threaded unit test suite.
3. Benchmark it against `std::queue + std::mutex`.
4. Add padding between hot atomics and measure whether it changes throughput.
5. Explain why `memory_order_relaxed` is sufficient for some operations but not for publishing data.
