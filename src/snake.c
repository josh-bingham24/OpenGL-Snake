#include <me/snake.h>
#include <me/shape_factory.h>
#include <cglm/cglm.h>
#include <stdlib.h>

Snake CreateSnake(unsigned int initialLength, float speed) {
    Snake snake = {
        .links = malloc(initialLength * sizeof(Shape*)),
        .length = initialLength,
        .speed = speed,
        .currentMovement = LEFT
    };

    float offset = 0.0f;
    for (unsigned int i = 0; i < initialLength; i++) {
        vec3 pos = { offset, 0.0f, 0.0f };
        snake.links[i] = CreateRectangle(1, 1, (vec3){ (float)(i + 0.5), -0.5f, 0.0f });
        offset += 2.0f;
    }

    return snake;
}

bool MoveSnake(Snake *snake) {
    vec3 previousPosition, temp;
    glm_vec3_copy(snake->links[0]->position, previousPosition);

    switch (snake->currentMovement) {
        case UP:
            if (snake->links[0]->position[1] >= 5.0f)
                snake->links[0]->position[1] *= -1.0f;
            else
                snake->links[0]->position[1] += snake->speed;
            break;
        case DOWN:
            if (snake->links[0]->position[1] <= -5.0f)
                snake->links[0]->position[1] *= -1.0f;
            else
                snake->links[0]->position[1] -= snake->speed;
            break;
        case LEFT:
            if (snake->links[0]->position[0] <= -7.0f)
                snake->links[0]->position[0] *= -1.0f;
            else
                snake->links[0]->position[0] -= snake->speed;
            break;
        case RIGHT:
            if (snake->links[0]->position[0] >= 7.0f)
                snake->links[0]->position[0] *= -1.0f;
            else
                snake->links[0]->position[0] += snake->speed;
            break;
    }

    if (snake->length > 1) {
        // check for collision
        for (unsigned int i = 1; i < snake->length; i++) {
            if (glm_vec3_eqv(snake->links[0]->position, snake->links[i]->position))
                return false;
        }

        for (unsigned int i = 1; i < snake->length; i++) {
            glm_vec3_copy(snake->links[i]->position, temp);
            glm_vec3_copy(previousPosition, snake->links[i]->position);
            glm_vec3_copy(temp, previousPosition);
        }
    }

    return true;
}

void GrowSnake(Snake *snake) {
    snake->length++;
    int length = snake->length;

    snake->links = realloc(snake->links, sizeof(Shape*) * length);
    snake->links[length - 1] = CreateRectangle(1, 1, snake->links[length - 2]->position);
}
