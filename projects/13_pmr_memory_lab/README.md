# 13 - PMR Memory Lab

Use polymorphic allocators to put many short-lived strings into a memory resource backed by one preallocated buffer.

## Concepts

- `<memory_resource>`
- `std::pmr::vector`
- `std::pmr::string`
- custom `memory_resource`
- allocation strategy vs container type
- lifetime/ownership of backing storage

## What to observe

The container API mostly looks normal. The difference is where it asks for memory. `pmr` makes the memory resource an object that can be chosen at runtime.

## Practice tasks

1. Instrument allocation count and bytes.
2. Compare `std::pmr::unsynchronized_pool_resource` with `monotonic_buffer_resource`.
3. Deliberately let the backing buffer die too early and explain the bug.
4. Store a struct with several PMR strings.
5. Write a small benchmark comparing default allocation to PMR allocation.
