#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdint.h>

typedef uint8_t Tile;

// Can be rendered by renderFloors in Floor.h or renderWalls in Wall.h.
// Values 0 <= x <= 127 are floors, 128 <= y <= 255 are walls.
typedef struct Map {
    int width;
    int height;
    Tile* tiles;
} Map;

Map* Map_Create(int width, int height);

void Map_Destroy(Map* map);

#endif