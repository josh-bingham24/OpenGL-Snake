#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef enum {
    CAM_FORWARD,
    CAM_BACKWARD,
    CAM_LEFT,
    CAM_RIGHT
} Direction;

typedef enum {
    STATIC,
    FLY,
    PLAYER
} CameraType;

typedef struct Camera {
    vec3 position,
        front,
        up;

    float yaw,
        pitch,
        fov,
        sensitivity;

    void (*Move)(struct Camera *camera, Direction direction, float speed);
} Camera;

Camera Camera_init(CameraType type, float fov, float sensitivity);
void HandleMouse(Camera *camera, double xoffset, double yoffset);
void HandleScroll(Camera *camera, double yoffset);

#endif