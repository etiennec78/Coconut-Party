#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "common.h"
#include "display.h"
#include "terrain.h"

// MARK: - Initialize game datas
void initGameDatas(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength, int isOptionsMenu) {

    if(!isOptionsMenu) {
        game->data.framerate = 30;
        game->data.soundEnabled = 1;
    }

    game->data.width = width;
    game->data.height = height;
    game->data.endHeight = (1 - LAND_WATER_RATIO) * height + WATER_MAX_RANDOMNESS + height * FINISH_LINE_RATIO;
    game->data.seed = seed;
    game->data.season = SPRING;
    game->data.minPathLength = minPathLength;
    game->data.maxPathLength = maxPathLength;
    game->data.crownHealth = 100;
    game->data.backoff.maxTime = 3;
    game->data.backoff.maxTries = 4;
    game->data.backoff.multiplier = 5;
    game->data.refreshDelay = 1e6 / game->data.framerate;
}

// MARK: - Skip scanf char
void emptyBuffer() {
    while(getchar() != '\n') {  }
}

// MARK: - Enable/Disable canonique mode
void setRawMode(int enable) {
    static struct termios oldConfigTer, newConfigTer;

    if(enable) {
        tcgetattr(STDIN_FILENO, &oldConfigTer);  // NOTE: Stock terminal config in oldConfigTer
        newConfigTer = oldConfigTer;
        newConfigTer.c_lflag &= ~(ICANON | ECHO); // NOTE: Disable ICANON (reading caract by caract) & ECHO (display caract)
        tcsetattr(STDIN_FILENO, TCSANOW, &newConfigTer); // NOTE: Set new configuration of terminal (TCSANOW apply modification now)
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldConfigTer); // NOTE: Restore old configuration of terminal (TCSANOW apply modification now)
    }
}
