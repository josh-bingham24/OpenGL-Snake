#ifndef SNAKE
#define SNAKE

#include <cglm/cglm.h>
#include <me/shape_factory.h>


// ----- Snake Movement -----
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} SnakeMovement;

// ----- Snake Object -----
typedef struct {
    Shape **links;
    unsigned int length;
    float speed;
    SnakeMovement currentMovement;
} Snake;


Snake CreateSnake(unsigned int initialLength, float speed);

bool MoveSnake(Snake *snake);
void GrowSnake(Snake *snake);


#endif