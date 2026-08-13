#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BANNED "iol"

void increment_password(char *password) {
    size_t selected = strlen(password) - 1;
    while (selected >= 0) {
        password[selected]++;
        if (password[selected] <= 'z')
            break;

        password[selected] = 'a';
        selected--;
    }
}

bool is_password_valid(const char *password) {
    bool straight = false;
    int pairs = 0;
    char pair_letter = '\0';
    for (const char *c = password; *c != '\0'; c++) {
        if (strchr(BANNED, *c) != NULL) {
            return false;
        }

        if (!straight && c[1] == *c + 1 && c[2] == *c + 2)
            straight = true;

        if (*c == c[1] && *c != pair_letter) {
            pair_letter = *c;
            pairs++;
        }
    }

    if (!straight || pairs < 2)
        return false;

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Correct usage: ./main <password>\n");
        return EXIT_SUCCESS;
    }

    do
        increment_password(argv[1]);
    while (!is_password_valid(argv[1]));
    printf("%s\n", argv[1]);
    return EXIT_SUCCESS;
}
