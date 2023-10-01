#include "Camera.h"

Camera* Camera_Create(int width, int height, double fov, double fogNear, 
    double fogFar) {
        
    Camera* camera = (Camera*) malloc(sizeof(Camera));
    
    fov *= 3.141592653589793 / 180.0;
    double aspect = (double) width / height;
    double tanFov2 = tan(fov / 2.0);
    double nearZ = aspect / (2.0 * tanFov2);
    
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->rotation = 0.0;
    camera->nearZ = nearZ;
    camera->frustumL.x = tanFov2;
    camera->frustumL.y = 1.0;
    camera->frustumR.x = -tanFov2;
    camera->frustumR.y = 1.0;
    camera->fogNear = fogNear;
    camera->fogFar = fogFar;
    camera->rays = (Vector*) malloc(width * sizeof(Vector));
    for (int x = 0; x < width; x++) {
        double t = (x + 0.5) / width;
        Vector ray;
        ray.x = (1.0 - t) * camera->frustumL.x +
            t * camera->frustumR.x;
        ray.y = camera->frustumL.y;
        camera->rays[x] = ray;
    }
    
    return camera;
}

void Camera_Destroy(Camera* camera) {
    if (camera == NULL)
        return;
    
    free(camera->rays);
    
    free(camera);
}