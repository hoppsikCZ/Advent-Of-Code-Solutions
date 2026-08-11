#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_naughty(char *line) {
    const char *p = line;
    if (*p == '\0' || *(p + 1) == '\0')
        return true;

    bool two_pairs = false;
    bool splitted_pair = false;
    while (*(p + 2) != '\0') {
        if (!two_pairs && *(p + 3) != '\0' &&
            memmem(p + 2, strlen(p + 2), p, 2) != NULL)
            two_pairs = true;

        if (!splitted_pair && *(p + 2) == *p)
            splitted_pair = true;

        if (splitted_pair && two_pairs)
            return false;

        p++;
    }

    // printf("%d %d", two_pairs, splitted_pair);
    return true;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");
    char line[20];

    unsigned int good_count = 0;
    while (fgets(line, sizeof(line), fr) != NULL) {
        line[strlen(line) - 1] = '\0';
        printf("%s\n", line);
        if (!is_naughty(line))
            good_count++;
    }

    printf("%u\n", good_count);
    return EXIT_SUCCESS;
}
