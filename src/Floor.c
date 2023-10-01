#include "Floor.h"

void renderFloors(Raster* screen, const Map* map, const Camera* camera, 
    const Raster* tileRasters[]) {
    
    Tile* tiles = map->tiles;
    int mapWidth = map->width;
    int mapHeight = map->height;
    
    uint8_t* pixels = screen->pixels;
    int width = screen->width;
    int height = screen->height;
    int height2 = (height + 1) / 2;
    int indexCeiling = 0;
    int indexFloor = 3 * width * (height - 1);
    int offset2Rows = 3 * 2 * width;
    
    double fogNear = camera->fogNear;
    double fogFar = camera->fogFar;
    double fogDiff = fogFar - fogNear;
    Vector pos = camera->position;
    double cosRot = cos(camera->rotation);
    double sinRot = sin(camera->rotation);
    Vector tmp;
    Vector left = camera->frustumL;
    tmp.x = left.x;
    tmp.y = left.y;
    left.x = tmp.x * cosRot + tmp.y * sinRot;
    left.y = tmp.y * cosRot - tmp.x * sinRot;
    Vector right = camera->frustumR;
    tmp.x = right.x;
    tmp.y = right.y;
    right.x = tmp.x * cosRot + tmp.y * sinRot;
    right.y = tmp.y * cosRot - tmp.x * sinRot;
    
    double zInv = 1.0 / camera->nearZ;
    double stepZInv = -zInv / height2;
    zInv += stepZInv * 0.5;
    for (int y = 0; y < height2; y++) {
        double z = 1 / zInv;
        double fogT = (fogFar - z) / fogDiff;
        fogT = fogT < 0.0 ? 0.0 :
               fogT > 1.0 ? 1.0 :
               fogT;
               
        Vector left_ = {
            .x = left.x * z,
            .y = left.y * z
        };
        Vector right_ = {
            .x = right.x * z,
            .y = right.y * z
        };
        Vector incr = {
            .x = (right_.x - left_.x) / width,
            .y = (right_.y - left_.y) / width
        };
        Vector uv = {
            .x = pos.x + left_.x + incr.x * 0.5,
            .y = pos.y + left_.y + incr.y * 0.5
        };
        for (int x = 0; x < width; x++) {
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
            
            int tileX = 
                uv.x < 0.0 ?
                (int) uv.x - 1 :
                (int) uv.x;
            int tileY =
                uv.y < 0.0 ? 
                (int) uv.y - 1 :
                (int) uv.y;
                    
            if (tileX >= 0 && tileX < mapWidth &&
                tileY >= 0 && tileY < mapHeight) {
                    
                Tile tile = tiles[tileX + tileY * mapWidth];
                if (tile < 128) {
                    const Raster* tileRaster = tileRasters[tile];
                    uint8_t* tilePixels = tileRaster->pixels;
                    int w = tileRaster->width;
                    int h = tileRaster->height;
                    int wh = w * h;
                    double u = uv.x - tileX;
                    double v = uv.y - tileY;
                    int px = (int) (u * w);
                    int py = (int) (v * h);
                    int idx = px + py * w;
                    if (idx >= 0 && idx < wh) {
                        idx *= 3;
                        r = (uint8_t) (fogT * tilePixels[idx + 0]);
                        g = (uint8_t) (fogT * tilePixels[idx + 1]);
                        b = (uint8_t) (fogT * tilePixels[idx + 2]);
                    }
                }
            }
            
            pixels[indexFloor + 0] = r;
            pixels[indexFloor + 1] = g;
            pixels[indexFloor + 2] = b;
            pixels[indexCeiling + 0] = r;
            pixels[indexCeiling + 1] = g;
            pixels[indexCeiling + 2] = b;
            
            uv.x += incr.x;
            uv.y += incr.y;
            indexCeiling += 3;
            indexFloor += 3;
        }
        zInv += stepZInv;
        indexFloor -= offset2Rows;
    }
}