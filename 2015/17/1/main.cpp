#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <vector>

int brute_force(std::vector<int> containers, int goal) {
    int result = 0;
    size_t max = 1;
    max = (max << containers.size());
    for (size_t i = 0; i < max; ++i) {
        int sum = 0;
        for (auto [j, container] : std::views::enumerate(containers)) {
            sum += container * ((i >> j) % 2);
        }

        if (sum == goal) {
            result++;
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file "
                  << (argc > 1 ? argv[1] : "../input.txt");
    }
    std::vector<int> input((std::istream_iterator<int>(file)),
                           std::istream_iterator<int>());

    std::println("{}", brute_force(input, 150));
    return 0;
}
