#include <fstream>
#include <iostream>
#include <print>
#include <vector>

constexpr int STEPS = 2503;

struct Reindeer {
    int speed{};
    int stamina{};
    int cooldown{};

    int calc_distance(int seconds) const {
        int distance = seconds / (stamina + cooldown) * speed * stamina;
        int reminder = seconds % (stamina + cooldown);
        distance += speed * (reminder > stamina ? stamina : reminder);
        return distance;
    }
};

std::vector<Reindeer> parse_data(std::istream &file) {
    std::vector<Reindeer> vec{};

    std::string discard{};
    int speed;
    int stamina;
    int cooldown;
    char discard_chr;
    std::string discart_str;

    while (file >> discard >> discard >> discard >> speed >> discard >>
           discard >> stamina >> discard >> discard >> discard >> discard >>
           discard >> discard >> cooldown >> discard) {
        vec.emplace_back(speed, stamina, cooldown);
    }

    return vec;
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto reindeers = parse_data(file);
    int max_distance = -1;
    for (const auto &reindeer : reindeers) {
        auto distance = reindeer.calc_distance(STEPS);
        if (distance > max_distance)
            max_distance = distance;
    }

    std::println(std::cout, "{}", max_distance);
    return 0;
}
