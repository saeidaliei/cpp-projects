# 15 - Type-Safe Units

Design a tiny units library so the compiler prevents accidental mixing of values such as meters and seconds.

## Concepts

- strong types through class templates
- `std::ratio`
- `consteval`
- user-defined literals
- `static_assert`
- compile-time conversion
- operator constraints

## Why this matters

A raw `double` cannot tell you whether `10.0` means ten meters, ten seconds, or ten kilograms. A quantity type moves that information into the type system.

## Practice tasks

1. Add kilograms and grams.
2. Add a conversion from kilometers per hour to meters per second.
3. Add multiplication of distance by a dimensionless scalar.
4. Prevent addition of incompatible units at compile time.
5. Explore `std::chrono` and compare its design to this miniature library.
