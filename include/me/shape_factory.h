#ifndef SHAPE_FACTORY
#define SHAPE_FACTORY

#include <cglm/cglm.h>

typedef struct {
    float *vertices;
    vec3 position;
    float rotation;
} Shape;


Shape CreateCircle(float radius, unsigned int edges, vec3 initialPosition);
Shape CreateRectangle(float width, float height, vec3 initialPosition);
void DestroyShape(Shape *shape);

#endif