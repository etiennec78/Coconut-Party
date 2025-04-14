#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "terrain.h"

char** allocateTerrain(int width, int height) {
    char** terrain = malloc(width * sizeof(char*));
    if (terrain == NULL) {
        exit(1);
    }

    for (int i = 0; i < width; i++) {
        terrain[i] = malloc(height * sizeof(char));
        if (terrain[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(terrain[j]);
            }
            free(terrain);
            exit(1);
        }
    }

    return terrain;
}

char** createTerrain(int width, int height) {
    char** terrain = allocateTerrain(width, height);
    srand(time(NULL));

    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
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

    return terrain;
}
