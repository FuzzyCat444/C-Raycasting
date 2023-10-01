#include "Map.h"

Map* Map_Create(int width, int height) {
    Map* map = (Map*) malloc(sizeof(Map));
    
    map->width = width;
    map->height = height;
    map->tiles = (Tile*) malloc(width * height * sizeof(Tile));
    
    return map;
}

void Map_Destroy(Map* map) {
    if (map == NULL)
        return;
    
    free(map->tiles);
    
    free(map);
}