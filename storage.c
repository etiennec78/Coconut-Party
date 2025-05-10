#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "asciiArt.h"
#include "display.h"
#include "storage.h"

#include <stdio.h>

// MARK: - Line counter
int countSavedGames() {
    long gamesListFileSize = 0;
    
    if(access(gamesListFilePath, F_OK) == -1) return -1; // NOTE: If the file does not exist, return -1

    FILE* gamesListFile = fopen(gamesListFilePath, "rb");
    if (gamesListFile == NULL) {
        printf("🚨 Failed to open file 'games-list.bin' (count saved games)\n");
        exit(1);
    }

    fseek(gamesListFile, 0, SEEK_END); // NOTE: Go to the end of the file

    gamesListFileSize = ftell(gamesListFile); // NOTE: Get the size of the file
    if (gamesListFileSize == -1) {
        printf("🚨 Failed to get file size 'games-list.bin' (count saved games)\n");
        fclose(gamesListFile);
        exit(1);
    }
    
    fclose(gamesListFile);
    return gamesListFileSize / sizeof(Header); // NOTE: Return the number of Header in the file
}

// MARK: - Listing game data
void listGame(Header* gamesHeaderList) {
    int headerCount = 0;

    FILE* gamesListFile = fopen(gamesListFilePath, "rb");
    if (gamesListFile == NULL) {
        printf("🚨 Failed to open file 'games-list.bin' (list saved games)\n");
        exit(1);
    }

    while (fread(&gamesHeaderList[headerCount], sizeof(Header), 1, gamesListFile)) { // NOTE: Read the file until the end and store all header in gamesHeaderList
        headerCount++;
    }

    fclose(gamesListFile);
}

// NOTE: Get the index of the header in the game list
int getHeaderIndex(int selectedGameID) {
    Header backupHeader;
    FILE *gamesListFile = NULL;
    int gameIDFound = 0, gameHeaderIndex = 0;
    
    gamesListFile = fopen(gamesListFilePath, "rb"); // NOTE: Open the file in read mode
    if(gamesListFile == NULL) {
        printf("🚨 Failed to open file 'games-list.bin' (restore game datas)\n");
        exit(1);
    }

    while(fread(&backupHeader, sizeof(Header), 1, gamesListFile)) { // NOTE: read saved headers
        if(backupHeader.id == selectedGameID) { // NOTE: If the game ID exists
            gameIDFound = 1;
            break;
        }

        gameHeaderIndex++;
        gameIDFound = 0;
    }

    fclose(gamesListFile);

    if(!gameIDFound) {
        printf("🚨 Game ID not found ! (SI#%d - BI#%d)\n", selectedGameID, backupHeader.id); // SI = selected game ID - BI = game backup ID
        exit(1);
    }

    return gameHeaderIndex;
}







// MARK: - Saved game datas
// NOTE: Set game name
void setGameName(Game* game, FILE* gamesListFile) {
    Header backupHeader;
    char gameName[GAME_NAME_LEN_MAX];
    int success = 0, gameNameFounded = 0;

    showCursor();
    setRawMode(0);

    do {
        printf("Enter the name of your game (max. 30 characters - without space): ");
        success = scanf("%30s", gameName); // NOTE: Limit input to 255 characters
        emptyBuffer(); // NOTE: Clear the input buffer

        while(success != 1 || strlen(gameName) == 0) {
            prevLine(1);
            clearLine();
            printf("Invalid input. Please enter a valid name (max. 30 characters - without space): ");

            success = scanf("%30s", gameName); // NOTE: Limit input to 255 characters
        }
        
        if(access(gamesListFilePath, F_OK) == 0) { // NOTE: If the file exists
            gamesListFile = fopen(gamesListFilePath, "r");
            if(gamesListFile == NULL) {
                printf("🚨 Failed to open file 'games-list.bin' (set game name)\n");
                exit(1);
            }

            while(fread(&backupHeader, sizeof(Header), 1, gamesListFile)) { // NOTE: read saved headers
                if(!strcmp(backupHeader.name, gameName)) { // NOTE: If the game name already exists
                    prevLine(1);
                    clearLine();
                    printf("The game name already exists.\n");

                    gameNameFounded = 1;
                    break;
                }

                gameNameFounded = 0;
            }

            fclose(gamesListFile);
        }
    } while(gameNameFounded); // NOTE: Check if the game name already exists
    
    hideCursor();
    setRawMode(1); // NOTE: Restore row mode

    strncpy(game->header.name, gameName, sizeof(game->header.name)); // NOTE: Copy the game name to the header
}

//NOTE: Prepare GameBackup structure
void prepareGameBackup(Game* game, GameBackup* gameBackup) {
    // NOTE: game.header
    gameBackup->gameHeader = game->header;

    // NOTE: game.data
    gameBackup->gameData = game->data;
    
    // NOTE: game.terrain
    for (int y=0; y<game->data.height; y++) {
        for (int x=0; x<game->data.width; x++) {
            gameBackup->gameTerrain[x][y] = game->terrain[x][y];
        }
    }
    
    // NOTE: game.path
    for (int p=0; p<game->path.length; p++) {
        gameBackup->gamePathTab[p] = game->path.tab[p];
    }
    gameBackup->gamePathLength = game->path.length;
    
    // NOTE: game.crown
    gameBackup->gameCrown = game->crown;

    // NOTE: game.crabs
    for (int c=0; c<game->crabs.length-1; c++) {
        gameBackup->gameCrabsTab[c] = game->crabs.tab[c];
    }
    gameBackup->gameCrabsLength = game->crabs.length;
    gameBackup->gameCrabsAwaitingSpawn = game->crabs.awaitingSpawn;
    gameBackup->gameCrabsNextSpawn = game->crabs.nextSpawn;
    gameBackup->gameCrabsRemaining = game->crabs.remaining;
}

// NOTE: Save game datas
void savedGame(Game* game) {
    FILE *gamesListFile = NULL, *gamesBackupFile = NULL;
    GameBackup gameBackup;

    clear();
    asciiArt("SaveGame");

    if(game->header.isAlreadySaved) {
        gamesBackupFile = fopen(gamesBackupFilePath, "rb+");
        if(gamesBackupFile == NULL) {
            printf("🚨 Failed to open file 'games-backup.bin' (saved game)\n");
            exit(1);
        }

        printf("Backup in progress... !\n");
        fseek(gamesBackupFile, getHeaderIndex(game->header.id) * sizeof(GameBackup), SEEK_SET);
    } else {
        setGameName(game, gamesListFile);
        
        gamesListFile = fopen(gamesListFilePath, "ab");
        if(gamesListFile == NULL) {
            printf("🚨 Failed to open file 'games-list.bin' (saved game)\n");
            exit(1);
        }

        gamesBackupFile = fopen(gamesBackupFilePath, "ab");
        if(gamesBackupFile == NULL) {
            printf("🚨 Failed to open file 'games-backup.bin' (saved game)\n");
            exit(1);
        }
    }
    
    prepareGameBackup(game, &gameBackup);

    // NOTE: Save game datas
    if(!game->header.isAlreadySaved) {
        gameBackup.gameHeader.isAlreadySaved = 1;
        fwrite(&gameBackup.gameHeader, sizeof(Header), 1, gamesListFile); // NOTE: Write game data in the bin file
        fclose(gamesListFile);
    }
    fwrite(&gameBackup, sizeof(GameBackup), 1, gamesBackupFile); // NOTE: Write game data in the bin file
    fclose(gamesBackupFile);
}







// MARK: - Restoring game data
//NOTE: Restore Game structure
void retoreGameStruct(Game* game, GameBackup gameBackup) {
    // NOTE: game.header
    game->header = gameBackup.gameHeader;
    
    // NOTE: game.data
    game->data = gameBackup.gameData;
    
    // NOTE: game.terrain
    for (int y=0; y<gameBackup.gameData.height; y++) {
        for (int x=0; x<gameBackup.gameData.width; x++) {
            game->terrain[x][y] = gameBackup.gameTerrain[x][y];
        }
    }
    
    // NOTE: game.path
    for (int p=0; p<gameBackup.gamePathLength; p++) {
        game->path.tab[p] = gameBackup.gamePathTab[p];
    }
    game->path.length = gameBackup.gamePathLength;
    
    // NOTE: game.crown
    game->crown = gameBackup.gameCrown;

    // NOTE: game.crabs
    for (int c=0; c<gameBackup.gameCrabsLength-1; c++) {
        game->crabs.tab[c] = gameBackup.gameCrabsTab[c];
    }
    game->crabs.length = gameBackup.gameCrabsLength;
    game->crabs.awaitingSpawn = gameBackup.gameCrabsAwaitingSpawn;
    game->crabs.nextSpawn = gameBackup.gameCrabsNextSpawn;
    game->crabs.remaining = gameBackup.gameCrabsRemaining;
}

// NOTE: Restore game datas
void restoreGame(Game* game, int selectedGameID) { // TODO: RAND_MAX
    Header backupHeader;
    GameBackup gameBackup;
    FILE *gamesListFile = NULL, *gamesBackupFile = NULL;

    int gameHeaderIndex = getHeaderIndex(selectedGameID);
    
    // NOTE: Restore game backup with the game header position 
    gamesBackupFile = fopen(gamesBackupFilePath, "rb"); // NOTE: Open the file in read mode
    if(gamesBackupFile == NULL) {
        printf("🚨 Failed to open file 'games-backup.bin' (retore game)\n");
        exit(1);
    }
    
    // NOTE: Read the game datas corresponding to the selected game
    fseek(gamesBackupFile, gameHeaderIndex * sizeof(GameBackup), SEEK_SET); // NOTE: Go to the game header position
    fread(&gameBackup, sizeof(GameBackup), 1, gamesBackupFile);

    if(gameBackup.gameHeader.id != selectedGameID) { // NOTE: Check if the game id is the same on the backup file
        printf("🚨 Game ID mismatch ! (SI#%d - BI#%d)\n", selectedGameID, gameBackup.gameHeader.id); // SI = selected game ID - BI = game backup ID
        exit(1);
    }

    retoreGameStruct(game, gameBackup); // NOTE: Restore game datas

    fclose(gamesBackupFile);
}