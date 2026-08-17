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

class Racer {
    Reindeer reindeer{};
    int distance{0};
    int timer{};
    bool resting{false};

    void flip_timer() {
        if (resting)
            timer = reindeer.stamina;
        else
            timer = reindeer.cooldown;
        resting = !resting;
    }

  public:
    int points{0};

    Racer(Reindeer reindeer, bool resting = false) : reindeer(reindeer) {
        if (resting)
            timer = reindeer.cooldown;
        else
            timer = reindeer.stamina;
    }

    int step() {
        if (!resting)
            distance += reindeer.speed;
        timer--;
        if (timer <= 0)
            flip_timer();
        return distance;
    }

    int step_to(int second) {
        distance = reindeer.calc_distance(second);
        return distance;
    }

    int get_distance() { return distance; }
};

std::vector<Reindeer> parse_data(std::istream &file) {
    std::vector<Reindeer> vec{};

    std::string discard{};
    int speed{};
    int stamina{};
    int cooldown{};

    while (file >> discard >> discard >> discard >> speed >> discard >>
           discard >> stamina >> discard >> discard >> discard >> discard >>
           discard >> discard >> cooldown >> discard) {
        vec.emplace_back(speed, stamina, cooldown);
    }

    return vec;
}

std::vector<Racer> prepare_racers(std::span<Reindeer> reindeers) {
    std::vector<Racer> racers{};

    for (const auto &reindeer : reindeers) {
        racers.emplace_back(reindeer);
    }

    return racers;
}

void assign_points(std::span<Racer> racers) {
    int winning_distance{0};
    std::vector<int> winners{};
    for (size_t i{0}; i < racers.size(); ++i) {
        auto racer = racers[i];
        auto distance = racer.get_distance();
        if (distance > winning_distance) {
            winners.clear();
            winners.push_back(i);
            winning_distance = distance;
        } else if (distance == winning_distance)
            winners.push_back(i);
    }

    for (const auto &winner : winners) {
        racers[winner].points++;
    }
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto reindeers = parse_data(file);
    auto racers = prepare_racers(reindeers);

    for (int i{1}; i <= STEPS; ++i) {
        for (auto &racer : racers) {
            racer.step();
            // racer.step_to(i);
        }
        assign_points(racers);
    }

    int max_points{0};
    for (const auto &racer : racers) {
        if (racer.points > max_points)
            max_points = racer.points;
    }

    std::println(std::cout, "{}", max_points);
    return 0;
}
