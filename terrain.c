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

    float ray1 = 0.75 * width / 2;
    float ray2 = 0.75 * height / 2;                 // Calculation of the horizontal and vertical half axis of ellipse

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
             
            float ellipse = ((y - x0) * (y - x0)) / (ray1 * ray1) + ((x - y0) * (x - y0)) / (ray2 * ray2)
                        + (rand() % 101)  / 1000;  // Calculation of the equation of the ellipse with a margin of +- 0.1

            if (ellipse < 1.0) {
                if (rand() % 2 == 0) {
                    terrain[x][y] = 0;
                } else {
                    terrain[x][y] = 1;
                }  
            } else {
                terrain[x][y] = 2;
            }
        }
    }
    return terrain;
}
