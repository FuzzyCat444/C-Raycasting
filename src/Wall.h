#ifndef WALL_H
#define WALL_H

#include <math.h>

#include "Map.h"
#include "Camera.h"
#include "Raster.h"
#include "Vector.h"

typedef struct RaycastResult {
    Tile tile;
    double z;
    double u;
} RaycastResult;

// Cast a ray in the map and find out which tile it hit, the z (depth) value
// of the hit, and the u texture coordinate of the hit.
// See https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm).
RaycastResult raycast(const Map* map, Vector position, Vector ray);

// Render walls of map to raster. tileRasters is an array of
// rasters where the tile ID will decide which raster is used.
void renderWalls(Raster* screen, const Map* map, const Camera* camera, 
    const Raster* tileRasters[]);

#endif