#include "../ingredient.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <istream>
#include <ranges>
#include <vector>

std::vector<Ingredient> parse_input(std::istream &file) {
    std::vector<Ingredient> ingredients{};
    int capacity{};
    int durability{};
    int flavor{};
    int texture{};
    int calories{};
    char discard_char;
    std::string discart_str;

    while (file >> discart_str >> discart_str >> capacity >> discard_char >>
           discart_str >> durability >> discard_char >> discart_str >> flavor >>
           discard_char >> discart_str >> texture >> discard_char >>
           discart_str >> calories) {
        ingredients.emplace_back(capacity, durability, flavor, texture,
                                 calories);
    }

    return ingredients;
}

int get_negatives(Ingredient &cookie) {
    int negatives = 0;
    if (cookie.capacity <= 0) {
        negatives += cookie.capacity - 1;
    }
    if (cookie.durability <= 0) {
        negatives += cookie.durability - 1;
    }
    if (cookie.flavor <= 0) {
        negatives += cookie.flavor - 1;
    }
    if (cookie.texture <= 0) {
        negatives += cookie.texture - 1;
    }

    return negatives;
}

long long calculate_score(Ingredient &cookie) {
    long long score{1};
    if (cookie.capacity > 0)
        score *= cookie.capacity;
    else
        score *= 0;
    if (cookie.durability > 0)
        score *= cookie.durability;
    else
        score *= 0;
    if (cookie.flavor > 0)
        score *= cookie.flavor;
    else
        score *= 0;
    if (cookie.texture > 0)
        score *= cookie.texture;
    else
        score *= 0;

    return score;
}

int add_best_ingredient(Ingredient &cookie,
                        const std::span<Ingredient> ingredients) {
    long long best_score = get_negatives(cookie);
    size_t best_ingredient{};
    for (auto [idx, ingredient] : std::ranges::views::enumerate(ingredients)) {
        auto temp_cookie = cookie + ingredient;
        int negatives = get_negatives(temp_cookie);
        long long score{};
        if (negatives < 0)
            score = negatives;
        else
            score = calculate_score(temp_cookie);

        if (score > best_score) {
            best_ingredient = idx;
            best_score = score;
        }
    }

    cookie += ingredients[best_ingredient];

    return best_ingredient;
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto ingredients = parse_input(file);
    Ingredient cookie{0, 0, 0, 0, 0};
    std::vector<int> ingredient_list{};
    for (int i{0}; i < 100; ++i) {
        ingredient_list.push_back(add_best_ingredient(cookie, ingredients));
    }

    std::println(std::cout, "{} {} {}", ingredient_list, cookie,
                 calculate_score(cookie));

    return 0;
}
