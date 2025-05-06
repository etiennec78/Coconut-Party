#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "terrain.h"
#include "display.h"

void createBackgroundEntities(Game* game) {
    game->backgroundEntities.length = 0;
    game->backgroundEntities.tab = NULL;
}

void addBackgroundEntity(Game* game, Coordinates coord, BackgroundEntityType type) {
    BackgroundEntity entity;
    entity.coord = coord;
    entity.type = type;
    entity.nextMove = 0;
    BackgroundEntities* entities = &game->backgroundEntities;

    entities->tab = realloc(entities->tab, sizeof(BackgroundEntity) * (entities->length + 1));
    if (entities->tab == NULL) {
        printf("Error: Failed to re-allocate background entities !\n");
        exit(1);
    }

    entities->tab[entities->length] = entity;
    entities->length++;
}

void setEntityNextCoordinates(Game* game, BackgroundEntity* entity) {
    int tilesLength = 0;
    Coordinates* surroundingTiles = getSurroundingTiles(game, entity->coord, &tilesLength);
    shuffleCoords(surroundingTiles, tilesLength);

    for (int i = 0; i < tilesLength; i++) {
        Coordinates tile = surroundingTiles[i];
        int x = tile.x;
        int y = tile.y;

        // If the entity is of land type
        if (entity->type <WATER_ENTITY_FIRST) {
            if (
                !coordsInPath(tile, game->path)
                && game->terrain[x][y] >= LAND_FIRST
                && game->terrain[x][y] <= LAND_LAST
            ) {
                entity->coord = tile;
                return;
            }

        // If the entity is of water type
        } else {
            if (game->terrain[x][y] == WATER) {
                entity->coord = tile;
                return;
            }
        }
    }
}

void updateBackgroundEntities(Game* game) {
    BackgroundEntity* entity;
    int pathIndex;
    int nextMoveIndex;
    int flooredSpeed;

    for (int i = 0; i < game->backgroundEntities.length; i++) {
        entity = &game->backgroundEntities.tab[i];

        if (entity->nextMove <= 0) {
            // Remove the old emoji
            printTerrainTile(game, entity->coord);

            // Move the entity
            setEntityNextCoordinates(game, entity);
            printBackgroundEntity(game, *entity);

            // Set the next movement
            float nextMoveTime = 0.5 + (rand() % 151) / 100.0; // 0.5 - 2s
            entity->nextMove = game->data.framerate * nextMoveTime;
        } else {
            entity->nextMove--;
        }
    }
}
