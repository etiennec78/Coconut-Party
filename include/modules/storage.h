#ifndef STORAGE_H
#define STORAGE_H

#include "core/common.h"

// MARK: - Constants
static const char gamesBackupFilePath[] = "./data/games-backup.bin";
static const char gamesListFilePath[] = "./data/games-list.bin";

// MARK: - Structures
typedef struct {
    // NOTE: game.header
    Header gameHeader;

    // NOTE: game.data
    Data gameData;

    // NOTE: game.score
    Score gameScore;
    
    // NOTE: game.terrain
    char gameTerrain[WIDTH_MAX][WIDTH_MAX * HEIGHT_MAX];

    // NOTE: game.path
    Coordinates gamePathTab[DEFAULT_MAX_PATH_LENGTH];
    int gamePathLength;
    
    // NOTE: game.crown
    Crown gameCrown;
    
    // NOTE: game.crabs
    Crab gameCrabsTab[DEFAULT_MAX_PATH_LENGTH];
    int gameWaveTotalCrabs;
    int gameCrabsLength;
    int gameCrabsAwaitingSpawn;
    int gameCrabsNextSpawn;
    int gameCrabsRemaining;

    // NOTE: game.monkeys
    Monkey gameMonkeysTab[DEFAULT_MAX_PATH_LENGTH * 2];
    int gameMonkeysLength;
    MonkeyShop gameShop;
    
    // NOTE: game.coins
    Coin gameCoinsTab[MAX_COINS];
    int gameCoinsLength;
    Coordinates gameCoinsScoreCoord;

    // NOTE: game.backgroundEntities
    BackgroundEntity gameBackgroundEntitiesTab[WIDTH_MAX * HEIGHT_MAX];
    int gameBackgroundEntitiesLength;

    // NOTE: game.end
    EndAnimation gameEnd;
} GameBackup;

// MARK: - Functions
int countSavedGames();
void listGame(Header* gamHeaderList);
int getHeaderIndex(int selectedGameID);

void setGameName(Game* game, FILE* gamesListFile);
void prepareGameBackup(Game* game, GameBackup* gameBackup);
void savedGame(Game* game);

void retoreGameStruct(Game* game, GameBackup* gameBackup);
void restoreGame(Game* game, int selectedGameID);
#endif