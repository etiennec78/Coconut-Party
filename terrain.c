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

    int x0 = width / 2;
    int y0 = height / 2;

    // Calculation of the half of horizontal and vertical rays of ellipse
    float ray1 = (width * LAND_WATER_RATIO) / 2;
    float ray2 = (height * LAND_WATER_RATIO) / 2;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            float ellipse = ((x - x0) * (x - x0)) / (ray1 * ray1) + ((y - y0) * (y - y0)) / (ray2 * ray2);

            // Generate a random number in [-0.05, 0.05]
            float randvalue = ((rand() % 1001) / 1000.0 - 0.5) * 2 * WATER_MAX_RANDOMNESS;

            if (ellipse + randvalue < 1.0) {
                switch (rand() % 2) {
                    case TREE1:
                        terrain[x][y] = 0;
                        break;
                    case TREE2:
                        terrain[x][y] = 1;
                        break;
                }
            } else {
                terrain[x][y] = 2;
            }
        }
    }
    return terrain;
}
