#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct Vector2D {
    int x;
    int y;
};

struct House {
    struct Vector2D location;
    int gift_count;
};

bool vecotor2d_equals(struct Vector2D *a, struct Vector2D *b) {
    // printf("comparing %d:%d and %d:%d\n", a->x, a->y, b->x, b->y);
    if (a->x == b->x && a->y == b->y)
        return true;

    return false;
}

int house_exists(struct House *house, size_t size, struct Vector2D *position) {
    for (size_t i = 0; i < size; i++) {
        if (vecotor2d_equals(&house[i].location, position))
            return i;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");
    if (fr == NULL) {
        printf("Failed to open file!");
        return -1;
    }

    int c;
    struct House *house_arr;
    size_t house_arr_size = 1;
    struct Vector2D pos = {0, 0};
    struct Vector2D pos_robo = {0, 0};

    bool robo_turn = false;

    house_arr = malloc(sizeof(struct House));
    house_arr[0] = (struct House){(struct Vector2D){0, 0}, 1};
    int test = 0;
    while ((c = fgetc(fr)) != EOF) {
        robo_turn = !robo_turn;
        if (c == '>')
            robo_turn ? pos_robo.x++ : pos.x++;
        else if (c == '<')
            robo_turn ? pos_robo.x-- : pos.x--;
        else if (c == '^')
            robo_turn ? pos_robo.y-- : pos.y--;
        else if (c == 'v')
            robo_turn ? pos_robo.y++ : pos.y++;
        else {
            continue;
        }
        // printf("Position is %d:%d\n", pos.x, pos.y);

        int i;
        if ((i = house_exists(house_arr, house_arr_size,
                              robo_turn ? &pos_robo : &pos)) >= 0) {
            house_arr[i].gift_count++;
        } else {
            // printf("Adding house number %zu on %d:%d\n", house_arr_size,
            // pos.x,
            // pos.y);
            house_arr_size++;
            house_arr =
                realloc(house_arr, sizeof(struct House) * house_arr_size);
            house_arr[house_arr_size - 1] =
                (struct House){robo_turn ? pos_robo : pos, 1};
        }
    }

    printf("%zu\n", house_arr_size);
    return EXIT_SUCCESS;
}
