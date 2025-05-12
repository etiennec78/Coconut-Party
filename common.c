#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "common.h"
#include "display.h"
#include "terrain.h"

const int MONKEY_PRICES[] = {5, 8, 12, 15, 15};

// MARK: - Initialize game data
void initGameData(Game *game, int width, int height, unsigned int seed, int minPathLength, int maxPathLength, int monkeyAmount, int crownHealth, int fromMenu) {
    game->header.id = rand();
    game->header.name[0] = '\0';
    game->header.isAlreadySaved = 0;

    if (!fromMenu) {
        game->data.framerate = 30;
        game->data.soundEnabled = 1;
    }

    game->data.width = width;
    game->data.height = height;
    game->data.endWidth = (1 - LAND_WATER_RATIO) * height + WATER_MAX_RANDOMNESS + height * FINISH_LINE_RATIO;
    game->data.seed = seed;
    game->data.season = SPRING;
    game->data.minPathLength = minPathLength;
    game->data.maxPathLength = maxPathLength;
    game->data.monkeyAmount = monkeyAmount;
    game->data.crownHealth = 80 + rand() % 41;

    game->end.nextMonkeyPop = 0;

    game->score.wave = 0;
    game->score.coins = 30;
    game->score.kills = 0;
    game->score.remainingCrabs = 0;

    game->data.backoff.maxTime = 3;
    game->data.backoff.maxTries = 4;
    game->data.backoff.multiplier = 5;

    game->data.refreshDelay = 1e6 / game->data.framerate;
}

// MARK: - Skip scanf character
void emptyBuffer() {
    while (getchar() != '\n');
}

// MARK: - Toggle canonical mode
void setRawMode(int enable) {
    static struct termios oldConfigTer, newConfigTer;

    if (enable) {
        // NOTE: Stock terminal config in oldConfigTer
        tcgetattr(STDIN_FILENO, &oldConfigTer);
        newConfigTer = oldConfigTer;

        // NOTE: Disable ICANON (reading caracter by caracter) & ECHO (display caracters)
        newConfigTer.c_lflag &= ~(ICANON | ECHO);

        // NOTE: Set new configuration of terminal (TCSANOW applies modifications in realtime)
        tcsetattr(STDIN_FILENO, TCSANOW, &newConfigTer);

    } else {
        // NOTE: Restore old configuration of terminal (TCSANOW applies modifications in realtime)
        tcsetattr(STDIN_FILENO, TCSANOW, &oldConfigTer);
    }
}
