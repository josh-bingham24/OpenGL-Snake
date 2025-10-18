#ifndef SHAPE_FACTORY
#define SHAPE_FACTORY

#include <cglm/cglm.h>

typedef struct {
    float *vertices;
    vec3 position;
} Shape;


Shape* CreateCircle(float radius, unsigned int edges, vec3 initialPosition);
Shape* CreateRectangle(float width, float height, vec3 initialPosition);
void DisposeShape(Shape *shape);

#endif