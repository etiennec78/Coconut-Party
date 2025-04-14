#include <stdlib.h>
#include <stdio.h>

#include "terrain.h"

void freeTerrain(char** terrain) {
    if (terrain != NULL) {
        if (terrain[0] != NULL) {
            free(terrain[0]);
        }
        free(terrain);
    }
}

char** allocateTerrain(int width, int height) {
    char* data = malloc(width * height * sizeof(char));
    if (data == NULL) {
        exit(1);
    }

    char** terrain = malloc(width * sizeof(char*));
    if (terrain == NULL) {
        free(data);
        exit(1);
    }

    for (int x = 0; x < width; x++) {
        terrain[x] = data + (x * height);
    }

    return terrain;
}

void createTerrain(Game* game) {
    char** terrain = allocateTerrain(game->data.width, game->data.height);
    srand(game->data.seed);

    for (int x = 0; x < game->data.width; x++) {
        for (int y = 0; y < game->data.height; y++) {
            switch (rand()%2) {
                case 0:
                    terrain[x][y] = 0;
                    break;
                case 1:
                    terrain[x][y] = 1;
                    break;
            }
        }
    }

    game->terrain = terrain;
}
