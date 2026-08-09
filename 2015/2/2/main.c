#include <stdio.h>
#include <stdlib.h>

int calc_ribbon(int a, int b, int c) {
    int ribbon;
    if (a >= b && a >= c)
        ribbon = 2 * (b + c);
    else if (b >= a && b >= c)
        ribbon = 2 * (a + c);
    else
        ribbon = 2 * (a + b);

    ribbon += a * b * c;
    return ribbon;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");
    int a, b, c, result = 0;
    while (fscanf(fr, "%dx%dx%d\n", &a, &b, &c) != EOF) {
        result += calc_ribbon(a, b, c);
    }
    printf("%d\n", result);
    return EXIT_SUCCESS;
}
