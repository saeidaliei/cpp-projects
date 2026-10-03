# 06 - File Indexer

Walk a directory tree and produce a small extension-based index. This is a practical introduction to the filesystem library.

## Concepts

- `std::filesystem::recursive_directory_iterator`
- error-code based APIs
- path manipulation
- aggregation into maps
- separation between traversal and reporting

## Run

```bash
./build/bin/cpp06_file_indexer .
```

For a nonexistent path, the program prints the filesystem error instead of throwing.

## Practice tasks

1. Add minimum/maximum file size per extension.
2. Ignore directories named `.git`, `build`, or `node_modules`.
3. Export the report as CSV.
4. Add a `--larger-than N` filter.
5. Refactor traversal so the caller supplies a callback for every matching file.
