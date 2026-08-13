#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CITIES 10
#define MAX_NAME_SIZE 20

struct Route {
    int val;
    struct City *city;
};

struct City {
    char name[MAX_NAME_SIZE];
    size_t route_count;
    struct Route routes[MAX_CITIES];
};

struct City *find_city(const struct City *const city_arr, size_t size,
                       char *name) {
    for (const struct City *city = city_arr; city - city_arr < size; city++) {
        if (!strcmp(city->name, name)) {
            return (struct City *)city;
        }
    }
    return NULL;
}

bool contains_city(const struct City *const haystack, size_t size,
                   const struct City *const needle) {
    for (const struct City *city = haystack; city - haystack < size; city++) {
        if (city == needle)
            return true;
    }

    return false;
}

bool contains_city_pointer(struct City *haystack[], size_t size,
                           const struct City *const needle) {
    for (size_t i = 0; i < size; i++) {
        if (haystack[i] == needle)
            return true;
    }

    return false;
}

size_t parse_date(FILE *const fr, struct City *city_arr, const size_t size) {
    char name1[MAX_NAME_SIZE];
    char name2[MAX_NAME_SIZE];
    int distance;
    size_t count = 0;
    while (count < size - 1 &&
           fscanf(fr, "%s to %s = %d", name1, name2, &distance) != EOF) {
        struct City *city1;
        if ((city1 = find_city(city_arr, count, name1)) == NULL) {
            city1 = city_arr + count++;
            city1->route_count = 0;
        }

        struct City *city2;
        if ((city2 = find_city(city_arr, count, name2)) == NULL) {
            city2 = city_arr + count++;
            city2->route_count = 0;
        }

        strcpy(city1->name, name1);
        struct Route *route1 = city1->routes + city1->route_count++;
        route1->val = distance;
        route1->city = city2;

        strcpy(city2->name, name2);
        struct Route *route2 = city2->routes + city2->route_count++;
        route2->val = distance;
        route2->city = city1;
    }

    return count;
}

int brute_shortest(struct City *city, struct City *cities, const size_t size,
                   struct City *visited[], size_t visited_size) {
    struct Route *routes = city->routes;
    visited[visited_size++] = city;

    int shortest = INT_MAX;

    for (struct Route *route = routes; route - routes < city->route_count;
         route++) {
        if (contains_city_pointer(visited, visited_size, route->city))
            continue;

        if (visited_size + 1 == size)
            return route->val;

        int shortest_finish =
            brute_shortest(route->city, cities, size, visited, visited_size) +
            route->val;
        if (shortest_finish < shortest)
            shortest = shortest_finish;
    }

    return shortest;
}

int main(int argc, char *argv[]) {
    FILE *fr = fopen(argc == 1 ? "../input.txt" : argv[1], "r");
    struct City cities[MAX_CITIES];
    size_t count = parse_date(fr, cities, MAX_CITIES);
    int shortest = INT_MAX;
    struct City *visited[MAX_CITIES];
    size_t visited_size;
    for (struct City *city = cities; city - cities < count; city++) {
        visited_size = 0;
        int length = brute_shortest(city, cities, count, visited, visited_size);
        if (length < shortest)
            shortest = length;
    }

    printf("%d\n", shortest);
    return EXIT_SUCCESS;
}
