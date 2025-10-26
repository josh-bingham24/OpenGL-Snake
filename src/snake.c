#include <me/snake.h>
#include <me/shape_factory.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include <time.h>

Snake CreateSnake(unsigned int initialLength, float speed) {
    Snake snake = {
        .links = malloc(initialLength * sizeof(Shape)),
        .length = initialLength,
        .speed = speed,
        .movements = { LEFT, INVALID, INVALID }
    };

    float offset = 0.0f;
    for (unsigned int i = 0; i < initialLength; i++) {
        vec3 pos = { offset, 0.0f, 0.0f };
        snake.links[i] = CreateRectangle(1, 1, (vec3){ (float)(i + 0.5), -0.5f, 0.0f });
        offset += 2.0f;
    }

    return snake;
}

void GrowSnake(Snake *snake) {
    snake->length++;
    int length = snake->length;

    snake->links = realloc(snake->links, sizeof(Shape) * length);
    snake->links[length - 1] = CreateRectangle(1, 1, snake->links[length - 2].position);
}

void ChangeSnakeDirection(Snake *snake, SnakeMovement newDirection) {
    unsigned int currentMod = snake->movements[0] % 2;
    unsigned int nextMod = snake->movements[1] % 2;
    unsigned int newMod = newDirection % 2;

    if (snake->movements[1] == INVALID) {
        if (currentMod != newMod)
            snake->movements[1] = newDirection;
    }
    else if (snake->movements[2] == INVALID) {
        if (nextMod != newMod)
            snake->movements[2] = newDirection;
    }
}

bool MoveSnake(Snake *snake, Shape *cheeseball) {
    vec3 previousPosition, temp;
    glm_vec3_copy(snake->links[0].position, previousPosition);

    if (snake->movements[2] != INVALID) {
        snake->movements[0] = snake->movements[1];
        snake->movements[1] = snake->movements[2];
        snake->movements[2] = INVALID;
    }
    else if (snake->movements[1] != INVALID) {
        snake->movements[0] = snake->movements[1];
        snake->movements[1] = INVALID;
    }

    switch (snake->movements[0]) {
        case UP:
            if (snake->links[0].position[1] >= 5.0f)
                snake->links[0].position[1] *= -1.0f;
            else
                snake->links[0].position[1] += snake->speed;
            break;
        case DOWN:
            if (snake->links[0].position[1] <= -5.0f)
                snake->links[0].position[1] *= -1.0f;
            else
                snake->links[0].position[1] -= snake->speed;
            break;
        case LEFT:
            if (snake->links[0].position[0] <= -7.0f)
                snake->links[0].position[0] *= -1.0f;
            else
                snake->links[0].position[0] -= snake->speed;
            break;
        case RIGHT:
            if (snake->links[0].position[0] >= 7.0f)
                snake->links[0].position[0] *= -1.0f;
            else
                snake->links[0].position[0] += snake->speed;
            break;
    }

    if (snake->length == 1)
        return true;

    for (unsigned int i = 1; i < snake->length; i++) {
        glm_vec3_copy(snake->links[i].position, temp);
        glm_vec3_copy(previousPosition, snake->links[i].position);
        glm_vec3_copy(temp, previousPosition);

        // check for collision with self
        if (glm_vec3_eqv(snake->links[0].position, snake->links[i].position))
            return false;
    }

    // check for collision with cheeseball
    if (glm_vec3_eqv(snake->links[0].position, cheeseball->position)) {
        GrowSnake(snake);
        MoveCheeseball(snake, cheeseball);
    }

    return true;
}

void MoveCheeseball(Snake *snake, Shape *cheeseball) {
    vec3 newPos;

    bool validPosition = false;
    while (!validPosition) {
        validPosition = true;

        // TODO: instead of generating a random position until we find a valid one, 
        // keep a list of available spaces, then pick a random position from that list
        // Each item in the list should be a vector
        float x = ((float)(rand() % 14) - 6) + 0.5f;
        float y = ((float)(rand() % 10) - 4) + 0.5f;
        glm_vec3_copy((vec3){ x, y, 0.0f }, newPos);

        for (unsigned int i = 0; i < snake->length; i++) {
            if (glm_vec3_eqv(snake->links[i].position, newPos))
                validPosition = false;
        }
    }

    glm_vec3_copy(newPos, cheeseball->position);
}

// CheeseballManager CreateCheeseballManager(unsigned int startAmount, Snake snake) {
//     CheeseballManager manager = {
//         .cheeseballs = malloc(startAmount * sizeof(Shape)),
//         .count = startAmount,
//         .validPositions = malloc((140 - snake.length) * sizeof(vec3))
//     };


// }
