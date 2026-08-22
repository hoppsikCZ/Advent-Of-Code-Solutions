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

    std::vector<std::string> generate_reaplacements(std::string_view original) {
        size_t pos = 0;
        std::vector<std::string> vec;
        while ((pos = original.find(input, pos)) != std::string_view::npos) {
            std::string copy(original);
            copy.replace(pos, input.size(), output);
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

int generate_cure(std::string_view input, std::vector<Action> actions) {
    std::vector<std::string> all_unique{"e"};
    std::vector<std::string> unique{"e"};
    int counter = 0;
    while (true) {
        counter++;
        std::vector<std::string> new_unique{};
        for (auto el : unique) {
            for (auto action : actions) {
                auto strings = action.generate_reaplacements(el);

                for (auto s : strings) {
                    if (s == input) {
                        return counter;
                    }
                    auto test =
                        std::find(all_unique.begin(), all_unique.end(), s);
                    if (test == all_unique.end()) {
                        new_unique.push_back(s);
                        all_unique.push_back(s);
                    }
                }
            }
        }
        unique = new_unique;
        std::cout << unique.size() << '\n';
    }

    return unique.size();
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto [start_string, actions] = parse_data(file);

    std::cout << generate_cure(start_string, actions) << '\n';
    return 0;
}
