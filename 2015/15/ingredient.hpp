#pragma once

#include <format>
struct Ingredient {
    int capacity{};
    int durability{};
    int flavor{};
    int texture{};
    int calories{};

    Ingredient &operator+=(const Ingredient &other);

    std::string to_string() const;

    friend Ingredient operator+(Ingredient left, const Ingredient &right);
};

template <>
struct std::formatter<Ingredient> : std::formatter<std::string_view> {
    auto format(const Ingredient &ingredient, std::format_context &ctx) const {
        return std::formatter<std::string_view>::format(ingredient.to_string(),
                                                        ctx);
    }
};
