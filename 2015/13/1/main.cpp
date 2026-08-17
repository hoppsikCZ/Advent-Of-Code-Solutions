#include <fstream>
#include <iostream>
#include <limits>
#include <print>
#include <span>
#include <vector>

struct Person;

struct Opinion {
    int val{};
    size_t person_id{};
};

struct Person {
    std::string name{};
    std::vector<Opinion> opinions{};
};

int find_person(const std::span<const Person> haystack,
                const std::string_view &needle) {
    for (int i{0}; i < haystack.size(); ++i) {
        if (haystack[i].name == needle)
            return i;
    }
    return -1;
}

std::vector<Person> parse_input(std::ifstream &file) {
    if (!file) {
        std::println(std::cout, "Failed to open file!");
    }
    std::vector<Person> ppl;
    std::string name;
    std::string target_name;
    std::string discard;
    std::string happy;
    int opinion{};
    while (file >> name >> discard >> happy >> opinion >> discard >> discard >>
           discard >> discard >> discard >> discard >> target_name) {

        // Remove trailing '.'
        target_name.pop_back();

        if (happy == "lose")
            opinion *= -1;

        auto person_id = find_person(ppl, name);
        if (person_id < 0) {
            ppl.emplace_back<Person>({name, {}});
            person_id = ppl.size() - 1;
        }

        auto target_id = find_person(ppl, target_name);
        if (target_id < 0) {
            ppl.emplace_back<Person>({target_name, {}});
            target_id = ppl.size() - 1;
        }

        ppl[person_id].opinions.emplace_back<Opinion>(
            {opinion, static_cast<size_t>(target_id)});
    }

    return ppl;
}

int find_opinion(const Person p, size_t op_pers_id) {
    for (const Opinion &op : p.opinions) {
        if (op.person_id == op_pers_id)
            return op.val;
    }

    return 0;
}

int calc_happy(const std::span<const Person> ppl, const size_t a,
               const size_t b) {
    return find_opinion(ppl[a], b) + find_opinion(ppl[b], a);
}

bool seated_contains(const std::span<const int> seated, size_t size,
                     int needle) {
    for (int i{0}; i < size; ++i) {
        if (needle == seated[i])
            return true;
    }

    return false;
}

int brute_max_happy(const std::span<Person> ppl, const std::span<int> seated,
                    size_t seated_size, size_t next) {
    int max = std::numeric_limits<int>::lowest();
    if (seated_size >= ppl.size() && seated_size != 0) {
        return calc_happy(ppl, seated[0], seated[seated_size - 1]);
    }

    for (int i{0}; i < ppl.size(); ++i) {
        if (seated_contains(seated, seated_size, i))
            continue;
        seated[seated_size] = i;
        int new_max = calc_happy(ppl, i, next) +
                      brute_max_happy(ppl, seated, seated_size + 1, i);
        if (new_max > max)
            max = new_max;
    }
    return max;
}

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");
    auto ppl = parse_input(file);
    std::vector<int> seated(ppl.size());
    int happy = brute_max_happy(ppl, seated, 1, 0);
    std::println(std::cout, "{}", happy);
    return 0;
}
