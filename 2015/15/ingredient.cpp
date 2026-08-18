#include "ingredient.hpp"

Ingredient &Ingredient::operator+=(const Ingredient &other) {
    capacity += other.capacity;
    durability += other.durability;
    flavor += other.flavor;
    texture += other.texture;
    calories += other.calories;

    return *this;
}

Ingredient operator+(Ingredient left, const Ingredient &right) {
    left += right;
    return left;
}

std::string Ingredient::to_string() const {
    return std::format("capacity: {} durability: {} flavor: {} "
                       "texture: {} calories: {}",
                       capacity, durability, flavor, texture, calories);
}
