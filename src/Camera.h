#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include <math.h>

#include "Vector.h"

typedef struct Camera {
    Vector position;
    double rotation;
    // z value at which the camera frustum intersects the floor
    double nearZ;
    // Vectors that represent the left and right sides of the view frustum.
    // Angle between these vectors is the fov angle.
    Vector frustumL;
    Vector frustumR;
    // Where the fog begins and ends.
    double fogNear;
    double fogFar;
    // 1D array of rays that will be cast to locate wall depth/texture coord.
    Vector* rays;
} Camera;

Camera* Camera_Create(int width, int height, double fov, double fogNear, 
    double fogFar);

void Camera_Destroy(Camera* camera);

#endif