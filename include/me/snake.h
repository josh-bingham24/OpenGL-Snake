#ifndef SNAKE
#define SNAKE

#include <cglm/cglm.h>
#include <me/shape_factory.h>


// ----- Snake Movement -----
typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    INVALID
} SnakeMovement;

// ----- Snake Object -----
typedef struct {
    Shape *links;
    unsigned int length;
    float speed;
    SnakeMovement movements[3];
} Snake;

// ----- Edible Item Manager Object -----
typedef struct {
    Shape *cheeseballs;
    vec3 *validPositions;
    unsigned int cheeseballCount;
    unsigned int validPositionCount;
} CheeseballManager;


Snake CreateSnake(unsigned int initialLength, float speed);
bool MoveSnake(Snake *snake, CheeseballManager *manager);
void ChangeSnakeDirection(Snake *snake, SnakeMovement newDirection);

CheeseballManager CreateCheeseballManager(unsigned int startAmount, Snake snake);
void MoveCheeseball(CheeseballManager *manager, int cheeseballIndex);


#endif