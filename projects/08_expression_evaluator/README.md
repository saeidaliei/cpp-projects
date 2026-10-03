# 08 - Expression Evaluator

Build a tiny arithmetic language supporting numbers, `+ - * /`, parentheses, unary minus, and variables.

## Concepts

- recursive descent parsing
- lexical scanning without a third-party parser
- AST representation with `std::variant`
- `std::visit`
- `std::unique_ptr`
- exception-based syntax errors

## Grammar

```text
expression := term (('+' | '-') term)*
term       := unary (('*' | '/') unary)*
unary      := '-' unary | primary
primary    := number | identifier | '(' expression ')'
```

## Practice tasks

1. Add exponentiation with `^` and correct precedence.
2. Add unary `+`.
3. Add built-in functions such as `sqrt(9)` and `abs(-2)`.
4. Parse the full source into an AST, then pretty-print the AST before evaluation.
5. Replace string variable lookup with `std::unordered_map<std::string, double>` injected into the evaluator.
