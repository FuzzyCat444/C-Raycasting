#include "Wall.h"

RaycastResult raycast(const Map* map, Vector position, Vector ray) {
    Tile* tiles = map->tiles;
    int width = map->width;
    int height = map->height;
    
    int signDx = ray.x < 0.0 ? -1 : 1;
    double dydx = ray.y / ray.x * signDx;
    double dtdx = 1.0 / ray.x * signDx;
    int signDy = ray.y < 0.0 ? -1 : 1;
    double dxdy = ray.x / ray.y * signDy;
    double dtdy = 1.0 / ray.y * signDy;
    
    int tileX = 
        position.x < 0.0 ? 
        (int) position.x - 1 :
        (int) position.x;
    int tileY = 
        position.y < 0.0 ?
        (int) position.y - 1 :
        (int) position.y;
    Tile tile = 0;
    double t = 0.0;
    double xy = 0.0;
    int hitAxis = -1;
    double t1 = 
        signDx == -1 ? 
        (position.x - tileX) * dtdx :
        (tileX + 1 - position.x) * dtdx;
    double y = position.y + ray.y * t1;
    double t2 =
        signDy == -1 ?
        (position.y - tileY) * dtdy :
        (tileY + 1 - position.y) * dtdy;
    double x = position.x + ray.x * t2;
    
    while (
        tileX >= 0 && tileX < width &&
        tileY >= 0 && tileY < height &&
        (tile = tiles[tileX + tileY * width]) < 128
    ) {
        if (t1 < t2) {
            t = t1;
            xy = y;
            t1 += dtdx;
            y += dydx;
            tileX += signDx;
            hitAxis = 0;
        } else {
            t = t2;
            xy = x;
            t2 += dtdy;
            x += dxdy;
            tileY += signDy;
            hitAxis = 1;
        }
    }
    
    RaycastResult result;
    result.tile = tile;
    result.z = t;
    if (hitAxis == 0) {
        if (signDx == -1) {
            result.u = tileY + 1 - xy;
        } else {
            result.u = xy - tileY;
        }
    } else if (hitAxis == 1) {
        if (signDy == -1) {
            result.u = xy - tileX;
        } else {
            result.u = tileX + 1 - xy;
        }
    } else {
        result.tile = 0;
    }
    return result;
}

void renderWalls(Raster* screen, const Map* map, const Camera* camera, 
    const Raster* tileRasters[]) {
    
    uint8_t* pixels = screen->pixels;
    int width = screen->width;
    int height = screen->height;
    double width2 = width / 2.0;
    double height2 = height / 2.0;
    
    double fogNear = camera->fogNear;
    double fogFar = camera->fogFar;
    double fogDiff = fogFar - fogNear;
    Vector position = camera->position;
    Vector* rays = camera->rays;
    Vector tmp;
    double cosRot = cos(camera->rotation);
    double sinRot = sin(camera->rotation);
    double p = camera->frustumL.x;
    
    for (int x = 0; x < width; x++) {
        Vector ray = rays[x];
        tmp.x = ray.x;
        tmp.y = ray.y;
        ray.x = tmp.x * cosRot + tmp.y * sinRot;
        ray.y = tmp.y * cosRot - tmp.x * sinRot;
        RaycastResult result = raycast(map, position, ray);
        if (result.tile >= 128) {
            double fogT = (fogFar - result.z) / fogDiff;
            fogT = fogT < 0.0 ? 0.0 :
                   fogT > 1.0 ? 1.0 :
                   fogT;
                
            const Raster* tileRaster = tileRasters[result.tile];
            uint8_t* tilePixels = tileRaster->pixels;
            int w = tileRaster->width;
            int h = tileRaster->height;
            int wh = w * h;
            
            double wallHeight = 1.0 / (p * result.z) * width2;
            double wallHeight2 = wallHeight / 2.0;
            double y1Exact = height2 - wallHeight2;
            int y1 = (int) (y1Exact + 0.5);
            int y2 = (int) (height2 + wallHeight2 + 0.5);
            int px = (int) (result.u * w);
            double vIncr = 1.0 / wallHeight;
            double v = (y1 + 0.5 - y1Exact) * vIncr;
            if (wallHeight > height) {
                y1 = 0;
                y2 = height;
                v = vIncr * (0.5 + wallHeight2 - height2);
            }
            int index = 3 * (x + width * y1);
            int indexIncr = 3 * width;
            for (int y = y1; y < y2; y++) {
                int py = (int) (v * h);
                int idx = px + py * w;
                
                if (idx >= 0 && idx < wh) {
                    idx *= 3;
                    pixels[index + 0] = (uint8_t) (fogT * tilePixels[idx + 0]);
                    pixels[index + 1] = (uint8_t) (fogT * tilePixels[idx + 1]);
                    pixels[index + 2] = (uint8_t) (fogT * tilePixels[idx + 2]);
                }
                
                v += vIncr;
                index += indexIncr;
            }
        }
    }
}