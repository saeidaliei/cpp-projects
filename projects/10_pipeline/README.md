# 10 - Producer/Consumer Pipeline

A three-stage text pipeline: generate records, transform them, and aggregate them. Bounded queues make back-pressure visible.

## Concepts

- producer/consumer architecture
- condition variables
- bounded queues
- cooperative shutdown with `std::jthread`
- RAII for synchronization objects
- ownership transfer with move semantics

## Practice tasks

1. Make the queue size configurable.
2. Add a fourth stage that writes results to a file.
3. Propagate exceptions between stages.
4. Add metrics for queue wait time.
5. Replace the mutex queue with a lock-free queue and compare complexity.
