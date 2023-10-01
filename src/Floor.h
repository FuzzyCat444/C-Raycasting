#ifndef FLOOR_H
#define FLOOR_H

#include <stdint.h>
#include <math.h>

#include "Map.h"
#include "Camera.h"
#include "Raster.h"
#include "Vector.h"

// Render floor and ceiling of map to raster. tileRasters is an array of
// rasters where the tile ID will decide which raster is used.
void renderFloors(Raster* screen, const Map* map, const Camera* camera, 
    const Raster* tileRasters[]);

#endif