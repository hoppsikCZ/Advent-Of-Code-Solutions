#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum operation {
    OPERATION_COPY,
    OPERATION_NOT,
    OPERATION_AND,
    OPERATION_OR,
    OPERATION_RSHIFT,
    OPERATION_LSHIFT,
};

union wire_input_option {
    uint16_t val;
    char key[3];
};

struct wire_input {
    enum operation op;
    union wire_input_option a;
    union wire_input_option b;
    bool a_is_val;
    bool b_is_val;
};

struct wire {
    uint16_t val;
    bool calculated;
    char key[3];
    struct wire_input in;
};

size_t read_file(char *path, struct wire *wire_map) {
    FILE *fr = fopen(path, "r");
    if (fr == NULL) {
        printf("Failed to open file %s!", path);
        exit(-1);
    }

    char line[50];
    size_t wire_count = 0;
    while (fgets(line, sizeof(line), fr) != NULL) {

        bool a_filled = false;
        bool in_set = false;
        char *token = strtok(line, " -\n");
        wire_map[wire_count].calculated = false;
        wire_map[wire_count].in.op = OPERATION_COPY;
        while (token != NULL) {
            if (*token <= '9') {
                if (!a_filled) {
                    wire_map[wire_count].in.a.val = atoi(token);
                    a_filled = true;
                    wire_map[wire_count].in.a_is_val = true;
                } else {
                    wire_map[wire_count].in.b.val = atoi(token);
                    wire_map[wire_count].in.b_is_val = true;
                }
            } else if (*token == '>') {
                in_set = true;
            } else if (*token <= 'Z') {
                enum operation op;
                if (strcmp(token, "NOT") == 0)
                    op = OPERATION_NOT;
                else if (strcmp(token, "AND") == 0)
                    op = OPERATION_AND;
                else if (strcmp(token, "OR") == 0)
                    op = OPERATION_OR;
                else if (strcmp(token, "RSHIFT") == 0)
                    op = OPERATION_RSHIFT;
                else
                    op = OPERATION_LSHIFT;

                wire_map[wire_count].in.op = op;
            } else if (!in_set) {
                if (!a_filled) {
                    strcpy(wire_map[wire_count].in.a.key, token);
                    a_filled = true;
                    wire_map[wire_count].in.a_is_val = false;
                } else {
                    strcpy(wire_map[wire_count].in.b.key, token);
                    wire_map[wire_count].in.b_is_val = false;
                }
            } else {
                strcpy(wire_map[wire_count].key, token);
            }

            // printf("Parsed token: %s\n", token);
            token = strtok(NULL, " -\n");
        }
        wire_count++;
        // printf("Parsed line %zu\n\n", wire_count);
    }

    fclose(fr);
    // printf("Done parsing!");
    return wire_count;
}

struct wire *find_key(const struct wire *const wire_map, size_t size,
                      const char *key) {
    for (struct wire *wire = (struct wire *)wire_map; wire < wire + size;
         wire++) {
        if (strcmp(wire->key, key) == 0)
            return wire;
    }

    return NULL;
}

uint16_t eval_wire(struct wire *const wire_map, size_t size,
                   struct wire *const wire);

uint16_t
eval_wire_input_option(struct wire *const wire_map, size_t size,
                       union wire_input_option *const wire_input_option,
                       bool is_val) {
    if (is_val)
        return wire_input_option->val;
    else
        return eval_wire(wire_map, size,
                         find_key(wire_map, size, wire_input_option->key));
}

uint16_t eval_wire_input(struct wire *const wire_map, size_t size,
                         struct wire_input *const wire_input) {
    switch (wire_input->op) {
    case OPERATION_COPY:
        return eval_wire_input_option(wire_map, size, &wire_input->a,
                                      wire_input->a_is_val);
    case OPERATION_NOT:
        return ~eval_wire_input_option(wire_map, size, &wire_input->a,
                                       wire_input->a_is_val);
    case OPERATION_AND:
        return eval_wire_input_option(wire_map, size, &wire_input->a,
                                      wire_input->a_is_val) &
               eval_wire_input_option(wire_map, size, &wire_input->b,
                                      wire_input->b_is_val);
    case OPERATION_OR:
        return eval_wire_input_option(wire_map, size, &wire_input->a,
                                      wire_input->a_is_val) |
               eval_wire_input_option(wire_map, size, &wire_input->b,
                                      wire_input->b_is_val);
    case OPERATION_RSHIFT:
        return eval_wire_input_option(wire_map, size, &wire_input->a,
                                      wire_input->a_is_val) >>
               eval_wire_input_option(wire_map, size, &wire_input->b,
                                      wire_input->b_is_val);
    case OPERATION_LSHIFT:
        return eval_wire_input_option(wire_map, size, &wire_input->a,
                                      wire_input->a_is_val)
               << eval_wire_input_option(wire_map, size, &wire_input->b,
                                         wire_input->b_is_val);
    }
}

uint16_t eval_wire(struct wire *const wire_map, size_t size,
                   struct wire *const wire) {
    if (wire->calculated)
        return wire->val;

    struct wire_input in = wire->in;

    wire->val = eval_wire_input(wire_map, size, &in);
    wire->calculated = true;
    return wire->val;
}

int main(int argc, char *argv[]) {
    struct wire wire_map[500];
    size_t count;
    if (argc == 1) {
        count = read_file("../input.txt", wire_map);
        printf("No input file argument provided, using ../input.txt\n");
    } else if (argc == 2) {
        count = read_file(argv[1], wire_map);
    }
    // printf("Data Parsed!");
    uint16_t result =
        eval_wire(wire_map, count, find_key(wire_map, count, "a"));

    printf("%u\n", result);

    return EXIT_SUCCESS;
}
