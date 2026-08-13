#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VOWELS "aeiou"

static const char *const banned_combinations[] = {
    "ab",
    "cd",
    "pq",
    "xy",
};

bool is_banned(char a, char b) {
    size_t banned_count =
        sizeof(banned_combinations) / sizeof(banned_combinations[0]);

    for (size_t i = 0; i < banned_count; i++) {
        const char *banned_comb = banned_combinations[i];
        if (a == banned_comb[0] && b == banned_comb[1])
            return true;
    }

    return false;
}

bool is_naughty(char *line) {
    char previous = line[0];
    if (previous == '\0')
        return true;

    int vowel_count = 0;
    if (strchr(VOWELS, previous) != NULL)
        vowel_count++;

    bool double_letter = false;
    for (const char *p = line + 1; *p != '\n'; p++) {
        char c = *p;

        if (is_banned(previous, c))
            return true;

        if (c == previous)
            double_letter = true;

        if (strchr(VOWELS, c) != NULL)
            vowel_count++;

        previous = c;
    }

    if (vowel_count >= 3 && double_letter)
        return false;

    return true;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");
    char line[20];

    unsigned int good_count = 0;
    while (fgets(line, sizeof(line), fr) != NULL) {
        printf("%s", line);
        if (!is_naughty(line))
            good_count++;
    }

    printf("%u\n", good_count);
    return EXIT_SUCCESS;
}
