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

    for (int i = 0; i < width; i++) {
        terrain[i] = data + (i * height);
    }

    return terrain;
}

void createTerrain(Game* game) {
    char** terrain = allocateTerrain(game->data.width, game->data.height);
    srand(game->data.seed);

    for (int i = 0; i < game->data.height; i++) {
        for (int j = 0; j < game->data.width; j++) {
            switch (rand()%2) {
                case 0:
                    terrain[i][j] = 0;
                    break;
                case 1:
                    terrain[i][j] = 1;
                    break;
            }
        }
    }

    game->terrain = terrain;
}
