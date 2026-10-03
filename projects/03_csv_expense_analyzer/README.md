# 03 - CSV Expense Analyzer

Parse a small CSV-like expense file and report totals by category. This project focuses on *explicit failure* instead of exceptions for routine input errors.

## Concepts

- `std::expected` (C++23)
- `std::from_chars`
- `std::string_view`
- parsing and validation
- `enum class`
- value-based error types

## Input format

```text
date,category,amount
2026-09-01,food,12.50
2026-09-02,transport,3.40
```

Run:

```bash
./build/bin/cpp03_csv_expense_analyzer examples.csv
```

The program will use an embedded sample if no file is supplied.

## Practice tasks

1. Support quoted CSV fields containing commas.
2. Validate dates into a small `Date` type instead of keeping strings.
3. Return line numbers in parse errors.
4. Make category an extensible value instead of an enum.
5. Write a second parser that returns `std::expected<std::vector<Expense>, ParseError>` and compare the APIs.
