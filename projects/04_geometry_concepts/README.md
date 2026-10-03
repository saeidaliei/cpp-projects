# 04 - Geometry & Concepts

A generic geometry mini-library using C++20 concepts. The purpose is to practice expressing requirements at the API boundary instead of relying on cryptic template errors.

## Concepts

- concepts and `requires`
- function templates
- constrained operators
- `constexpr`
- `std::numbers`
- aggregate/value types
- generic algorithms

## Practice tasks

1. Add a `Circle<T>` with `area()` and `perimeter()`.
2. Add a concept requiring `x` and `y` to be arithmetic.
3. Make `centroid()` work for any range of points.
4. Add a `Rectangle<T>` and a generic `bounding_box` function.
5. Make all area calculations `constexpr` and prove them with `static_assert`.
