#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <mdspan>

constexpr size_t HEIGHT = 100;
constexpr size_t WIDTH = 100;

struct Range {
    int min{};
    int max{};
};

class Board {
    Range survive;
    Range born;
    std::array<bool, HEIGHT * WIDTH> arr_{};
    std::mdspan<bool, std::dextents<size_t, 2>> board_;

    int calc_neighbours(int x, int y) {
        int neighbours = 0;
        for (int i = y - 1; i <= y + 1; ++i) {
            if (i >= 0 && i < HEIGHT) {
                for (int j = x - 1; j <= x + 1; ++j) {
                    if (j >= 0 && j < WIDTH && !(x == j && y == i) &&
                        board_[i, j]) {
                        neighbours++;
                    }
                }
            }
        }

        return neighbours;
    }

  public:
    Board(Range survive, Range born, std::istream &input)
        : survive(survive), born(born), board_(arr_.data(), HEIGHT, WIDTH) {
        char ch;
        size_t x = 0, y = 0;
        while (input.get(ch)) {
            if (ch == '\n') {
                y++;
                x = 0;
            } else if (x >= WIDTH || y >= HEIGHT) {
                std::cerr << "Invalid input data";
                break;
            } else {
                board_[y, x] = (ch == '#');
                x++;
            }
        }

        print_board();
    }

    void step(int count) {
        while (count > 0) {
            std::array<bool, HEIGHT * WIDTH> temp = arr_;
            auto temp_mdsppan = std::mdspan(temp.data(), HEIGHT, WIDTH);
            std ::cout << "steps left " << count << std::endl;
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    if ((x == 0 || x == WIDTH - 1) &&
                        (y == 0 || y == HEIGHT - 1)) {
                        continue;
                    }
                    int neighbours = calc_neighbours(x, y);
                    if ((neighbours < survive.min ||
                         neighbours > survive.max)) {
                        temp_mdsppan[y, x] = false;
                    } else if (neighbours >= born.min &&
                               neighbours <= born.max) {
                        temp_mdsppan[y, x] = true;
                    }
                }
            }

            arr_ = temp;
            board_ = std::mdspan(arr_.data(), HEIGHT, WIDTH);
            count--;
            print_board();
        }
    }

    int count_alive() {
        int alive = 0;
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (board_[y, x]) {
                    alive++;
                }
            }
        }
        return alive;
    }

    void print_board() {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                std::cout << (board_[y, x] ? '#' : '.');
            }
            std::cout << '\n';
        }
    }
};

int main(int argc, char *argv[]) {
    std::ifstream file(argc > 1 ? argv[1] : "../input.txt");

    Board board({2, 3}, {3, 3}, file);

    board.step(100);
    std::cout << board.count_alive() << '\n';
    return 0;
}
