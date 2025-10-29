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

    Shape *tmp = realloc(snake->links, sizeof(Shape) * length);
    if (tmp != NULL) {
        snake->links = tmp;
    } else {
        printf("Error reallocating snake links\n");
    }

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

bool MoveSnake(Snake *snake, CheeseballManager *manager) {
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

    // check for collision with cheeseball
    bool collisionDetected = false;
    for (unsigned int i = 0; i < manager->cheeseballCount; i++) {
        if (glm_vec3_eqv(snake->links[0].position, manager->cheeseballs[i].position)) {
            MoveCheeseball(manager, i);
            GrowSnake(snake);
            collisionDetected = true;

            break;
        }
    }

    for (unsigned int i = 1; i < snake->length; i++) {
        glm_vec3_copy(snake->links[i].position, temp);
        glm_vec3_copy(previousPosition, snake->links[i].position);
        glm_vec3_copy(temp, previousPosition);

        // check for collision with self
        if (glm_vec3_eqv(snake->links[0].position, snake->links[i].position))
            return false;
    }

    // update valid positions
    if (!collisionDetected) {
        for (unsigned int i = 0; i < manager->validPositionCount; i++) {
            if (glm_vec3_eqv(manager->validPositions[i], snake->links[0].position)) {
                glm_vec3_copy(temp, manager->validPositions[i]);
                break;
            }
        }
    }

    return true;
}


void MoveCheeseball(CheeseballManager *manager, int cheeseballIndex) {
    // handle case where no more valid positions
    if (manager->validPositionCount == 0) {
        // move offscreen
        glm_vec3_copy((vec3){ 10.0f, 10.0f, 0.0f }, manager->cheeseballs[cheeseballIndex].position);
        return;
    }

    int posIndex = rand() % manager->validPositionCount;
    manager->validPositionCount--;

    glm_vec3_copy(manager->validPositions[posIndex], manager->cheeseballs[cheeseballIndex].position);

    // remove vector from valid positions
    for (unsigned int j = posIndex; j < manager->validPositionCount; j++) {
        glm_vec3_copy(manager->validPositions[j + 1], manager->validPositions[j]);
    }

    vec3 *tmp = realloc(manager->validPositions, manager->validPositionCount * sizeof(vec3));
    if (tmp != NULL || manager->validPositionCount == 0) {
        manager->validPositions = tmp;
    } else {
        printf("Error reallocating valid positions\n");
    }
}


CheeseballManager CreateCheeseballManager(unsigned int startAmount, Snake snake) {
    CheeseballManager manager = {
        .cheeseballs = malloc(startAmount * sizeof(Shape)),
        .validPositions = malloc((192 - snake.length) * sizeof(vec3)),
        .cheeseballCount = startAmount,
        .validPositionCount = 192 - snake.length
    };

    // create array of valid positions
    float x = -7.5f;
    float y = 5.5f;
    unsigned int validIndex = 0;
    for (unsigned int i = 0; i < 192; i++) {
        vec3 vec = { x, y, 0.0f };

        // make sure position is a part of snake
        bool isSnake = false;
        for (unsigned int j = 0; j < snake.length; j++) {
            if (glm_vec3_eqv(snake.links[j].position, vec))
                isSnake = true;
        }

        if (!isSnake) {
            glm_vec3_copy(vec, manager.validPositions[validIndex]);
            validIndex++;
        }

        if (x == 7.5f){
            x = -7.5f;
            y -= 1.0f;
        }
        else {
            x += 1.0f;
        }
    }

    // create cheeseball(s)
    for (unsigned int i = 0; i < startAmount; i++) {
        manager.cheeseballs[i] = CreateCircle(0.3f, 36, (vec3){ 0.0f, 0.0f, 0.0f });

        MoveCheeseball(&manager, i);
    }

    return manager;
}
