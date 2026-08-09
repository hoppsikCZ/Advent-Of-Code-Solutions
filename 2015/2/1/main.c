#include <stdio.h>
#include <stdlib.h>

int area_with_slack(int a, int b, int c) {
    int side_a = a * b;
    int side_b = a * c;
    int side_c = b * c;

    int slack;
    if (side_a <= side_b && side_a <= side_c)
        slack = side_a;
    else if (side_b <= side_a && side_b <= side_c)
        slack = side_b;
    else
        slack = side_c;

    return 2 * (side_a + side_b + side_c) + slack;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");
    int a, b, c, result = 0;
    while (fscanf(fr, "%dx%dx%d\n", &a, &b, &c) != EOF) {
        result += area_with_slack(a, b, c);
    }
    printf("%d\n", result);
    return EXIT_SUCCESS;
}
