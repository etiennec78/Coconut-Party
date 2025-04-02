#include "terrain.h"

#define WIDTH 40
#define HEIGHT 30

int main() {
    char** terrain = createTerrain(WIDTH, HEIGHT);
    free(terrain);
    return 0;
}
