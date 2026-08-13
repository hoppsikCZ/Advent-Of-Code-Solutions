#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fr = fopen(argc > 1 ? argv[1] : "../input.txt", "r");
    if (fr == NULL) {
        printf("Invalid file!\n");
        return EXIT_FAILURE;
    }

    int c;
    int sum = 0;
    int current = 0;
    bool negative = false;
    while ((c = fgetc(fr)) != EOF) {
        if (c == '-')
            negative = true;
        else if (c >= '0' && c <= '9') {
            current *= 10;
            current += c - '0';
        } else if (current != 0) {
            if (negative)
                current *= -1;

            sum += current;
            current = 0;
            negative = false;
        }
    }

    printf("%d\n", sum);
    return EXIT_SUCCESS;
}
