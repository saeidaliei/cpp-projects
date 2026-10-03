# 09 - Thread Pool

Implement a small thread pool using C++20 `std::jthread`, a task queue, condition variables, and futures.

## Concepts

- `std::jthread`
- `std::stop_token`
- `std::condition_variable_any`
- `std::packaged_task` / `std::future`
- mutex-protected queues
- cooperative shutdown

## Important concurrency idea

The pool does not kill worker threads. Destruction marks the pool as stopping, asks each `std::jthread` to stop, wakes waiting workers, and then relies on `std::jthread`'s RAII join behavior.

Also notice the interaction between the two shutdown signals: the pool-level `stopping_` flag handles the shared queue state, while the thread's stop token is the thread-local cancellation mechanism.

## Practice tasks

1. Add `size()` and queue-length statistics.
2. Add a `submit_void()` helper for tasks without a return value.
3. Measure speedup for CPU-heavy work with 1, 2, 4, and 8 workers.
4. Add priorities to tasks.
5. Deliberately introduce a data race in a separate branch, observe it with ThreadSanitizer, then fix it.
