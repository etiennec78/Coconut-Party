#ifndef STORAGE_H
#define STORAGE_H

#include "common.h"

// MARK: - Constants
static const char gamesBackupFilePath[] = "./data/games_backups.bin";
static const char gamesListFilePath[] = "./data/backups_list.bin";

// MARK: - Structures
typedef struct {
    char gameName[GAME_NAME_LEN_MAX];
    
    // NOTE: game.header
    Header gameHeader;

    // NOTE: game.data
    Data gameData;
    
    // NOTE: game.terrain
    char gameTerrain[WIDTH_MAX][WIDTH_MAX * HEIGHT_MAX];

    // NOTE: game.path
    Coordinates gamePathTab[DEFAULT_MAX_PATH_LENGHT];
    int gamePathLength;
    
    // NOTE: game.crown
    Crown gameCrown;
    
    // NOTE: game.crabs
    Crab gameCrabsTab[DEFAULT_MAX_PATH_LENGHT];
    int gameCrabsLength;
    int gameCrabsAwaitingSpawn;
    int gameCrabsNextSpawn;
    int gameCrabsRemaining;
} GameBackup;

// MARK: - Functions
int countSavedGames();
void listGame(Header* gamHeaderList);
int getHeaderIndex(int selectedGameID);

void setGameName(Game* game, FILE* gamesListFile);
void prepareGameBackup(Game* game, GameBackup* gameBackup);
void savedGame(Game* game);

void retoreGameStruct(Game* game, GameBackup gameBackup);
void restoreGame(Game* game, int selectedGameID);
#endif