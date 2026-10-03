# 05 - LRU Cache

Implement a generic least-recently-used cache using a hash map plus a doubly linked list.

## Concepts

- class templates
- iterators and references
- `std::list` iterator stability
- `std::unordered_map`
- `std::optional`
- exception-safe/value-oriented APIs

## Why this design?

The hash map gives average O(1) lookup. The list gives O(1) splice-to-front and eviction from the back. The key is that list iterators remain valid when other list elements are moved.

## Practice tasks

1. Add a `contains()` method.
2. Add a `reserve()` method that reserves hash-map capacity.
3. Add statistics for hits, misses, and evictions.
4. Make the cache capacity changeable at runtime.
5. Decide whether returning a copy from `get()` is the right API and experiment with alternatives.
