#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");

    int c;
    int answer = 0;
    while ((c = fgetc(fr)) != EOF) {
        if (c == '(') {
            answer++;
        } else if (c == ')') {
            answer--;
        }
    }

    printf("%d\n", answer);
    return EXIT_SUCCESS;
}
