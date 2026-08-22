#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct Action {
    std::string input{};
    std::string output{};

    std::vector<std::string> generate_reaplacements(std::string_view original,
                                                    bool reverse = false) {
        size_t pos = 0;
        std::vector<std::string> vec;
        while ((pos = original.find(reverse ? output : input, pos)) !=
               std::string_view::npos) {
            std::string copy(original);
            copy.replace(pos, reverse ? output.size() : input.size(),
                         reverse ? input : output);
            vec.push_back(copy);
            pos++;
        }
        return vec;
    }
};

std::pair<std::string, std::vector<Action>> parse_data(std::istream &input) {
    std::vector<Action> actions{};

    std::string start_string;
    for (std::string line; std::getline(input, line);) {
        std::string s1, s2, discard;
        std::istringstream stream(line);
        if (stream >> s1 >> discard >> s2) {
            actions.emplace_back(s1, s2);
        } else if (!s1.empty()) {
            start_string = s1;
        }
    }
    return {start_string, actions};
}

int generate_cure_recursive(std::string_view input,
                            const std::vector<Action> &actions,
                            std::vector<std::string> &all_unique,
                            std::string_view goal) {
    // std::cout << input << '\n';

    for (auto action : actions) {
        auto strings = action.generate_reaplacements(input, true);

        for (auto s : strings) {
            if (s == goal) {
                // std::cout << goal << '\n';
                return 1;
            }
            auto test = std::find(all_unique.begin(), all_unique.end(), s);
            if (test == all_unique.end()) {
                all_unique.push_back(s);
                auto rec_result =
                    generate_cure_recursive(s, actions, all_unique, goal);

                if (rec_result >= 0) {
                    return rec_result + 1;
                }
            }
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto [start_string, actions] = parse_data(file);
    std::sort(actions.begin(), actions.end(), [](Action a, Action b) {
        return a.output.size() - a.input.size() >
               b.output.size() - b.input.size();
    });
    std::vector<std::string> all_unique{"e"};
    std::vector<std::string> unique{"e"};

    std::cout << generate_cure_recursive(start_string, actions, all_unique, "e")
              << '\n';
    return 0;
}
