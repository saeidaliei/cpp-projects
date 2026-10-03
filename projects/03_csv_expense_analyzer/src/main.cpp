#include <array>
#include <charconv>
#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Expense {
    std::string date;
    std::string category;
    long long cents{};
};

struct ParseError {
    std::size_t line{};
    std::string message;
};

std::expected<long long, std::string> parse_cents(std::string_view text) {
    const auto dot = text.find('.');
    std::string_view whole = text;
    std::string_view fraction = "00";
    if (dot != std::string_view::npos) {
        whole = text.substr(0, dot);
        fraction = text.substr(dot + 1);
    }
    if (fraction.size() > 2 || (fraction.empty() && dot != std::string_view::npos)) {
        return std::unexpected("bad decimal part");
    }

    long long major{};
    const auto [ptr, ec] = std::from_chars(whole.data(), whole.data() + whole.size(), major);
    if (ec != std::errc{} || ptr != whole.data() + whole.size()) {
        return std::unexpected("bad whole-number part");
    }

    int minor = 0;
    if (!fraction.empty()) {
        const auto [fptr, fec] = std::from_chars(fraction.data(), fraction.data() + fraction.size(), minor);
        if (fec != std::errc{} || fptr != fraction.data() + fraction.size()) {
            return std::unexpected("bad fractional part");
        }
    }
    if (fraction.size() == 1) minor *= 10;
    return major * 100 + minor;
}

std::expected<Expense, ParseError> parse_row(std::string_view line, std::size_t number) {
    std::array<std::string_view, 3> fields{};
    std::size_t start = 0;
    for (std::size_t i = 0; i < fields.size(); ++i) {
        const auto comma = line.find(',', start);
        if (i < fields.size() - 1 && comma == std::string_view::npos) {
            return std::unexpected(ParseError{number, "expected three columns"});
        }
        const auto end = (i == fields.size() - 1) ? line.size() : comma;
        fields[i] = line.substr(start, end - start);
        start = end + 1;
    }

    auto cents = parse_cents(fields[2]);
    if (!cents) {
        return std::unexpected(ParseError{number, "invalid amount: " + cents.error()});
    }
    if (fields[0].empty() || fields[1].empty()) {
        return std::unexpected(ParseError{number, "date and category are required"});
    }

    return Expense{std::string(fields[0]), std::string(fields[1]), *cents};
}

int main(int argc, char** argv) {
    const std::string sample =
        "date,category,amount\n"
        "2026-09-01,food,12.50\n"
        "2026-09-02,transport,3.40\n"
        "2026-09-03,food,7.00\n";

    std::string data;
    if (argc >= 2) {
        std::ifstream input(argv[1]);
        if (!input) {
            std::cerr << "Could not open " << argv[1] << '\n';
            return 1;
        }
        data.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    } else {
        data = sample;
    }

    std::istringstream lines(data);
    std::string line;
    std::getline(lines, line); // header

    std::unordered_map<std::string, long long> totals;
    std::size_t line_number = 1;
    while (std::getline(lines, line)) {
        ++line_number;
        if (line.empty()) continue;

        auto expense = parse_row(line, line_number);
        if (!expense) {
            std::cerr << "Line " << expense.error().line << ": "
                      << expense.error().message << '\n';
            continue;
        }
        totals[expense->category] += expense->cents;
    }

    std::cout << "Totals by category:\n";
    for (const auto& [category, cents] : totals) {
        std::cout << "  " << category << ": " << cents / 100 << '.'
                  << (cents % 100 < 10 ? "0" : "") << cents % 100 << '\n';
    }
}
