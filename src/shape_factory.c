#include <me/shape_factory.h>
#include <stdlib.h>
#include <cglm/cglm.h>



void DisposeShape(Shape *shape);



Shape CreateCircle(float radius, unsigned int edges, vec3 initialPosition) {
    Shape circle;

    glm_vec3_copy(initialPosition, circle.position);

    float *vertices = malloc(3 * 5 * edges * sizeof(float));

    float step = 360.0f / edges;
    unsigned int degrees = 0;
    for (int i = 0; i < edges; i++) {
        float currentSine = sin(glm_rad(degrees));
        float currentCosine = cos(glm_rad(degrees));
        float nextSine = sin(glm_rad(degrees + step));; 
        float nextCosine = cos(glm_rad(degrees + step));
        
        vec3 currentPoint = {
            currentSine,
            currentCosine,
            0.0f
        };
        glm_vec3_scale(currentPoint, radius, currentPoint);

        vec3 nextPoint = {
            nextSine,
            nextCosine,
            0.0f
        };
        glm_vec3_scale(nextPoint, radius, nextPoint);

        // center
        vertices[i * 15 + 0] = 0.0f;
        vertices[i * 15 + 1] = 0.0f;
        vertices[i * 15 + 2] = 0.0f;
        vertices[i * 15 + 3] = 0.5f;
        vertices[i * 15 + 4] = 0.5f;

        // current
        vertices[i * 15 + 5] = currentPoint[0];
        vertices[i * 15 + 6] = currentPoint[1];
        vertices[i * 15 + 7] = currentPoint[2];
        vertices[i * 15 + 8] = currentSine * 0.5 + 0.5;
        vertices[i * 15 + 9] = currentCosine * 0.5 + 0.5;

        // next
        vertices[i * 15 + 10] = nextPoint[0];
        vertices[i * 15 + 11] = nextPoint[1];
        vertices[i * 15 + 12] = nextPoint[2];
        vertices[i * 15 + 13] = nextSine * 0.5 + 0.5;
        vertices[i * 15 + 14] = nextCosine * 0.5 + 0.5;


        degrees += step;
    }
    circle.vertices = vertices;

    return circle;
}

Shape CreateRectangle(float width, float height, vec3 initialPosition) {
    Shape rectangle;
    float *vertices = malloc(6 * 5 * sizeof(float));

    glm_vec3_copy(initialPosition, rectangle.position);

    float x = width / 2;
    float y = height / 2;

    float points[] = {
        x, -y,
        -x, y,
        -x, -y,
        x, -y,
        x, y,
        -x, y
    };

    for (unsigned int i = 0; i < 6; i++) {
        float currentX = points[2 * i];
        float currentY = points[2 * i + 1];
        float xText, yText;

        if (currentX > 0 && currentY > 0) {
            xText = 1.0f;
            yText = 1.0f;
        }
        else if (currentX < 0 && currentY > 0) {
            xText = 0.0f;
            yText = 1.0f;
        }
        else if (currentX > 0 && currentY < 0) {
            xText = 1.0f;
            yText = 0.0f;
        }
        else {
            xText = 0.0f;
            yText = 0.0f;
        }

        vertices[i * 5 + 0] = currentX;
        vertices[i * 5 + 1] = currentY;
        vertices[i * 5 + 2] = 0.0f;
        vertices[i * 5 + 3] = xText;
        vertices[i * 5 + 4] = yText;
    }

    rectangle.vertices = vertices;

    return rectangle;
}

void DisposeShape(Shape *shape) {
    free(shape->vertices);
}
