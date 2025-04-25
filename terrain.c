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
    float a = (width * LAND_WATER_RATIO) / 2;
    float b = (height * LAND_WATER_RATIO) / 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
           
            float ellipse = ((j - x0) * (j - x0)) / (a * a) + ((i - y0) * (i - y0)) / (b * b);
            
            // Generate a random number in [-0.05, 0.05]
            float randvalue = ((rand() % 1001) / 1000.0 - 0.5) * 2 * WATER_MAX_RANDOMNESS;

            if (ellipse + randvalue < 1) {
                switch (rand() % 2) {
                    case 0:
                        terrain[i][j] = 0;  
                        break;
                    case 1:
                        terrain[i][j] = 1; 
                        break;
                }
            } else {
                terrain[i][j] = 2;  
            }
        }
    }

    return terrain;
}
