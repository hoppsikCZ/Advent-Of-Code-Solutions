#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 1000
#define COLS 1000

struct Rect {
    size_t top;
    size_t left;
    size_t bottom;
    size_t right;
};

enum state {
    STATE_OFF,
    STATE_ON,
    STATE_TOGGLE,
};

void set_light_range(size_t cols, bool lights[][cols],
                     const struct Rect *bounds, enum state new_state) {
    for (size_t y = bounds->top; y <= bounds->bottom; y++) {
        for (size_t x = bounds->left; x <= bounds->right; x++) {
            switch (new_state) {
            case STATE_OFF:
                lights[y][x] = false;
                break;
            case STATE_ON:
                lights[y][x] = true;
                break;
            case STATE_TOGGLE:
                lights[y][x] = !lights[y][x];
                break;
            }
        }
    }
}

struct Rect parse_line(char *line, enum state *out_state) {
    size_t top, left, bottom, right;
    char state_str[10];
    sscanf(line, "%9[^0-9]%lu,%lu%*[^0-9]%lu,%lu", state_str, &top, &left,
           &bottom, &right);

    // printf("%lu,%lu %lu,%lu %s", top, left, bottom, right, state_str);
    if (strstr(state_str, "on") != NULL) {
        *out_state = STATE_ON;
    } else if (strstr(state_str, "off") != NULL) {
        *out_state = STATE_OFF;
    } else {
        *out_state = STATE_TOGGLE;
    }

    // printf(" %d", *out_state);
    return (struct Rect){top, left, bottom, right};
}

size_t count_lights(size_t rows, size_t cols, const bool lights[rows][cols]) {
    size_t count = 0;
    for (size_t y = 0; y < rows; y++) {
        for (size_t x = 0; x < cols; x++) {
            if (lights[y][x])
                count++;
        }
    }

    return count;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen("../input.txt", "r");
    if (fr == NULL) {
        printf("Failed to open input.txt!\n");
        return -1;
    }

    bool lights[ROWS][COLS];
    memset(lights, 0, sizeof(lights));

    char line[100];
    while (fgets(line, sizeof(line), fr) != NULL) {
        enum state new_state;
        struct Rect bounds = parse_line(line, &new_state);
        // printf("%s, bounds: %lu,%lu %lu,%lu\n", line, bounds.top,
        // bounds.left,
        //        bounds.bottom, bounds.right);
        set_light_range(COLS, lights, &bounds, new_state);
    }

    size_t light_count = count_lights(ROWS, COLS, lights);
    printf("%lu\n", light_count);

    return EXIT_SUCCESS;
}
