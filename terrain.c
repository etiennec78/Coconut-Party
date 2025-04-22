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

    float a = 0.75 * width / 2;
    float b = 0.75 * height / 2;                 // Calculation of the horizontal and vertical half axis of ellipse

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
             
            float value = ((j - x0) * (j - x0)) / (a * a) + ((i - y0) * (i - y0)) / (b * b)
                        + (rand() % 101)  / 1000;  // Calculation of the equation of the ellipse with a margin of +- 0.1

            if (value < 1.0) {
                if (rand() % 2 == 0) {
                    terrain[i][j] = 0;
                } else {
                    terrain[i][j] = 1;
                }  
            } else {
                terrain[i][j] = 2;  
            }
        }
    }
    return terrain;
}
