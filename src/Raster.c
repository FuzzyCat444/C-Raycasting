#include "Raster.h"

Raster* Raster_Create(int width, int height) {
    Raster* raster = (Raster*) malloc(sizeof(Raster));
    
    raster->width = width;
    raster->height = height;
    raster->pixels = (uint8_t*) malloc(3 * width * height * sizeof(uint8_t));
    
    return raster;
}

void Raster_Destroy(Raster* raster) {
    if (raster == NULL)
        return;
    
    free(raster->pixels);
    
    free(raster);
}