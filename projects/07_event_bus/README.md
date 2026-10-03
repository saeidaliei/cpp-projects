# 07 - Event Bus

A tiny in-process event system. Subscribers can listen for strongly typed events, while the bus stores callbacks behind a type-erased interface.

## Concepts

- `std::function`
- `std::any`
- `std::type_index`
- type erasure
- templates with runtime dispatch
- lambda capture and lifetimes

## Important design question

`subscribe<Event>` is compile-time typed, but the storage is heterogeneous. The bus solves that mismatch by erasing the exact event type at storage boundaries and restoring it with `std::any_cast` during dispatch.

## Practice tasks

1. Return a subscription token and support unsubscribe.
2. Support one-shot subscriptions.
3. Add event priorities.
4. Add a thread-safe event bus and document what synchronization protects.
5. Replace `std::any` with a custom type-erased wrapper and compare the complexity.
