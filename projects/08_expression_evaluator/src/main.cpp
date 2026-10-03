#include <concepts>
#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

struct Number;
struct Variable;
struct Unary;
struct Binary;

using Expr = std::variant<double, std::string,
                          std::unique_ptr<Unary>, std::unique_ptr<Binary>>;

struct Unary {
    char op{};
    Expr operand;
};

struct Binary {
    char op{};
    Expr left;
    Expr right;
};

class Parser {
public:
    explicit Parser(std::string_view source) : source_(source) {}

    Expr parse() {
        Expr result = expression();
        skip_ws();
        if (!at_end()) {
            throw std::runtime_error("Unexpected input at position " + std::to_string(pos_));
        }
        return result;
    }

private:
    Expr expression() {
        Expr left = term();
        while (true) {
            skip_ws();
            if (peek('+') || peek('-')) {
                const char op = source_[pos_++];
                left = std::make_unique<Binary>(Binary{op, std::move(left), term()});
            } else {
                return left;
            }
        }
    }

    Expr term() {
        Expr left = unary();
        while (true) {
            skip_ws();
            if (peek('*') || peek('/')) {
                const char op = source_[pos_++];
                left = std::make_unique<Binary>(Binary{op, std::move(left), unary()});
            } else {
                return left;
            }
        }
    }

    Expr unary() {
        skip_ws();
        if (peek('-')) {
            ++pos_;
            return std::make_unique<Unary>(Unary{'-', unary()});
        }
        return primary();
    }

    Expr primary() {
        skip_ws();
        if (at_end()) {
            throw std::runtime_error("Expected expression");
        }
        if (peek('(')) {
            ++pos_;
            Expr result = expression();
            skip_ws();
            if (at_end() || source_[pos_] != ')') {
                throw std::runtime_error("Missing ')' ");
            }
            ++pos_;
            return result;
        }
        if (std::isdigit(static_cast<unsigned char>(source_[pos_]))) {
            std::size_t consumed = 0;
            const double value = std::stod(std::string(source_.substr(pos_)), &consumed);
            pos_ += consumed;
            return value;
        }
        if (std::isalpha(static_cast<unsigned char>(source_[pos_]))) {
            std::string name;
            while (!at_end() && std::isalnum(static_cast<unsigned char>(source_[pos_]))) {
                name.push_back(source_[pos_++]);
            }
            return name;
        }
        throw std::runtime_error("Unexpected character at position " + std::to_string(pos_));
    }

    void skip_ws() {
        while (!at_end() && std::isspace(static_cast<unsigned char>(source_[pos_]))) {
            ++pos_;
        }
    }

    bool at_end() const noexcept { return pos_ >= source_.size(); }
    bool peek(char ch) const noexcept { return !at_end() && source_[pos_] == ch; }

    std::string_view source_;
    std::size_t pos_{};
};

double evaluate(const Expr& expr, const std::unordered_map<std::string, double>& vars) {
    return std::visit([&](const auto& node) -> double {
        using T = std::decay_t<decltype(node)>;
        if constexpr (std::same_as<T, double>) {
            return node;
        } else if constexpr (std::same_as<T, std::string>) {
            const auto it = vars.find(node);
            if (it == vars.end()) throw std::runtime_error("Unknown variable: " + node);
            return it->second;
        } else if constexpr (std::same_as<T, std::unique_ptr<Unary>>) {
            const double value = evaluate(node->operand, vars);
            return node->op == '-' ? -value : value;
        } else {
            const double left = evaluate(node->left, vars);
            const double right = evaluate(node->right, vars);
            switch (node->op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return left / right;
                default: throw std::runtime_error("Unknown operator");
            }
        }
    }, expr);
}

int main() {
    const std::vector<std::string> examples{
        "2 + 3 * 4",
        "(x + 5) * 2",
        "-7 + 3"
    };
    const std::unordered_map<std::string, double> vars{{"x", 10}};

    for (const std::string& source : examples) {
        try {
            const Expr ast = Parser(source).parse();
            std::cout << source << " = " << evaluate(ast, vars) << '\n';
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << '\n';
        }
    }
}
