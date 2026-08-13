#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");

    int c;
    int answer = 0;
    int pos = 0;

    while ((c = fgetc(fr)) != EOF) {
        pos++;
        if (c == '(') {
            answer++;
        } else if (c == ')') {
            answer--;
        }

        if (answer < 0)
            break;
    }

    printf("%d\n", pos);
    return EXIT_SUCCESS;
}
