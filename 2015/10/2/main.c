#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "3113322113"
#define STEPS 50

char *generate_instuctions(const char *text) {
    size_t text_size = strlen(text);
    char *new_text = malloc(text_size * sizeof(text[0]));
    size_t new_text_size = text_size;
    size_t new_text_length = 0;
    char prev_char = *text;
    int match_count = 1;
    for (const char *c = text + 1; c - text <= text_size; c++) {
        if (prev_char == *c)
            match_count++;
        else {
            if (new_text_length + 2 > new_text_size) {
                new_text = realloc(new_text, new_text_size *= 2);
                if (new_text == NULL) {
                    exit(EXIT_FAILURE);
                }
            }

            new_text[new_text_length] = match_count + '0';
            new_text[new_text_length + 1] = prev_char;
            new_text_length += 2;

            match_count = 1;
            prev_char = *c;
        }
    }
    new_text[new_text_length] = '\0';

    return new_text;
}

int main(int argc, char *argv[]) {
    char *text = malloc(sizeof(INPUT));
    strcpy(text, INPUT);
    for (int i = 0; i < STEPS; i++) {
        char *new_text;
        new_text = generate_instuctions(text);
        free(text);
        text = new_text;
    }

    printf("%zu\n", strlen(text));
    free(text);

    return EXIT_SUCCESS;
}
