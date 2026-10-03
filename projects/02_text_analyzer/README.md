# 02 - Text Analyzer

Analyze a text file with C++20/23 ranges. The interesting part is not the counting itself; it is composing lazy views into a readable pipeline.

## Concepts

- `<ranges>` and lazy views
- `std::views::split`
- `std::string_view`
- lambda expressions
- algorithms and projections
- `unordered_map`
- structured bindings

## Run

```bash
./build/bin/cpp02_text_analyzer path/to/file.txt
```

If you omit a filename, a built-in sample is analyzed.

## Read the pipeline carefully

A range pipeline should make the transformation stages visible: split, normalize, filter, then aggregate. Views generally do not own data and are evaluated when iterated.

## Practice tasks

1. Add a top-10 word list sorted by frequency.
2. Ignore a configurable stop-word file.
3. Report average word length.
4. Count distinct words case-insensitively without allocating one string per token.
5. Rewrite the implementation so the aggregation logic accepts any input range.
