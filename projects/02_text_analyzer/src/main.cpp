#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

std::string lower_copy(std::string_view input) {
    std::string result(input);
    std::ranges::transform(result, result.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return result;
}

std::string load_text(int argc, char** argv) {
    if (argc < 2) {
        return "Ranges make data processing readable. Ranges can be lazy, reusable, and composable.";
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input) {
        throw std::runtime_error("Could not open input file");
    }
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

int main(int argc, char** argv) {
    const std::string text = load_text(argc, argv);
    std::unordered_map<std::string, std::size_t> counts;
    std::size_t token_count = 0;

    // std::views::split does not create a container of strings. Each item is a
    // subrange, so we can decide exactly when to materialize a std::string.
    auto words = text
        | std::views::split(' ')
        | std::views::transform([](auto&& part) {
              std::string word;
              for (const char ch : part) {
                  if (std::isalnum(static_cast<unsigned char>(ch))) {
                      word.push_back(ch);
                  }
              }
              return word;
          })
        | std::views::filter([](const std::string& word) { return !word.empty(); });

    for (std::string word : words) {
        ++token_count;
        ++counts[lower_copy(word)];
    }

    std::vector<std::pair<std::string, std::size_t>> sorted(counts.begin(), counts.end());
    std::ranges::sort(sorted, [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second > b.second;
        }
        return a.first < b.first;
    });

    std::cout << "Tokens: " << token_count << "\n"
              << "Unique words: " << counts.size() << "\n\n";
    for (const auto& [word, count] : sorted | std::views::take(10)) {
        std::cout << word << " -> " << count << '\n';
    }
}
