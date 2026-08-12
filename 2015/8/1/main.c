#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEXADEC_CHARS "0123456789abcdef"

int read_file(FILE *fr) {
    if (fr == NULL)
        return -1;

    char line[50];
    int result = 0;
    while (fgets(line, sizeof(line), fr) != NULL) {
        line[strcspn(line, "\n")] = 0;

        int count_of_chars = 0;
        // -2 to account for "" around each line
        int count_of_escaped = -2;
        int escaped_lock = 0;
        for (char *c = line; *c != '\0'; c++) {
            count_of_chars++;

            if (escaped_lock > 0) {
                escaped_lock--;
            } else {
                count_of_escaped++;
                if (*c == '\\') {
                    if (c[1] == '\\' || c[1] == '\"')
                        escaped_lock = 1;
                    else if (c[1] == 'x' &&
                             strchr(HEXADEC_CHARS, c[2]) != NULL &&
                             strchr(HEXADEC_CHARS, c[3]) != NULL)
                        escaped_lock = 3;
                }
            }
        }
        result += count_of_chars - count_of_escaped;
    }

    return result;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen(argc == 1 ? "../input.txt" : argv[1], "r");
    int result = read_file(fr);
    printf("%d\n", result);
    return EXIT_SUCCESS;
}
