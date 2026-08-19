#include <charconv>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

constexpr std::string delimiters = " ,:";

std::vector<std::string_view> tokenize(std::string_view sv,
                                       std::string_view delimiters) {
    std::vector<std::string_view> tokens{};
    size_t start = 0;

    while ((start = sv.find_first_not_of(delimiters, start)) !=
           std::string_view::npos) {
        auto end = sv.find_first_of(delimiters, start);
        tokens.push_back(sv.substr(start, end - start));
        start = end;
    }

    return tokens;
}

typedef std::vector<std::unordered_map<std::string, int>> Sue;

std::vector<std::unordered_map<std::string, int>>
parse_input(std::istream &input) {
    std::vector<std::unordered_map<std::string, int>> vec;
    std::string line;
    while (std::getline(input, line)) {
        auto tokens = tokenize(line, delimiters);
        std::unordered_map<std::string, int> things;
        std::string_view name{};
        for (auto [i, token] : std::views::enumerate(tokens)) {
            if (i < 2)
                continue;

            if (i % 2) {
                int val;
                auto [ptr, err] = std::from_chars(
                    token.data(), token.data() + token.size(), val);
                if (err == std::errc{} && ptr == token.data() + token.size()) {
                    things[std::string(name)] = val;
                } else {
                    std::println(std::cerr,
                                 "Failed to parse input with error: {}",
                                 std::make_error_code(err).message());
                }
            } else {
                name = token;
            }
        }

        vec.push_back(things);
    }

    return vec;
}

bool is_sue_valid(std::unordered_map<std::string, int> conditions,
                  std::unordered_map<std::string, int> sue) {
    for (auto &[key, val] : sue) {
        if (conditions[key] != val) {
            return false;
        }
    }

    return true;
}

size_t find_sue(std::vector<std::unordered_map<std::string, int>> sues,
                std::unordered_map<std::string, int> conditions) {
    for (auto [id, sue] : std::views::enumerate(sues)) {
        for (auto &[key, val] : sue) {
            if (is_sue_valid(conditions, sue)) {
                return id;
            }
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto sues = parse_input(file);
    std::unordered_map<std::string, int> sue{
        {"children", 3}, {"cats", 7},     {"samoyeds", 2}, {"pomeranians", 3},
        {"akitas", 0},   {"vizslas", 0},  {"goldfish", 5}, {"trees", 3},
        {"cars", 2},     {"perfumes", 1},
    };

    // sues start wiht index of 1
    auto result = find_sue(sues, sue) + 1;
    std::println(std::cout, "{}", result);
    return 0;
}
