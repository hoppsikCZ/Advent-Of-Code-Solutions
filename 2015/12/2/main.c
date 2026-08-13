#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RECURSIVE -1

int main(int argc, char *argv[]) {
    FILE *fr;
    if (argc == RECURSIVE) {
        fr = (FILE *)argv;
    } else {
        fr = fopen(argc > 1 ? argv[1] : "../input.txt", "r");
        if (fr == NULL) {
            printf("Invalid file!\n");
            return EXIT_FAILURE;
        }
    }

    int c;
    int sum = 0;
    int current = 0;
    bool negative = false;
    int red_deepness = -1;
    int deepness = 0;
    int red_counter = 0;
    bool red_active = false;
    int staged = 0;
    while ((c = fgetc(fr)) != EOF) {
        if (c >= '0' && c <= '9') {
            current *= 10;
            current += c - '0';
        } else {
            if (current != 0) {
                if (negative)
                    current *= -1;
                staged += current;
                current = 0;
                negative = false;
            }

            switch (c) {
            case '{':
                deepness++;
                if (deepness != 1 && argc != RECURSIVE || argc == RECURSIVE)
                    staged += main(RECURSIVE, (char **)fr);
                break;
            case '}':
                deepness--;
                if (!red_active) {
                    sum += staged;
                } else if (deepness < red_deepness)
                    red_active = false;

                staged = 0;

                if (argc == RECURSIVE)
                    return sum;
                break;
            case '-':
                negative = true;
                break;
            case ':':
                if (!red_active)
                    red_counter = 1;
                break;
            default:
                if (!red_active && red_counter != 0) {
                    if (red_counter == 1 && c == '\"' ||
                        red_counter == 2 && c == 'r' ||
                        red_counter == 3 && c == 'e' ||
                        red_counter == 4 && c == 'd')
                        red_counter++;
                    else if (red_counter == 5 && c == '\"') {
                        red_counter = 0;
                        red_active = true;
                        red_deepness = deepness;
                    } else
                        red_counter = 0;
                }
            }
        }
    }

    if (argc == RECURSIVE)
        return sum;

    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
