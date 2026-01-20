#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>

#define INITIAL_SNAKE_LENGTH 3;

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  Position *body;
  int length;
  int dx;
  int dy;
} Snake;

Snake create_snake(void);
void destroy_snake(Snake *s);

void move_snake(Snake *s);
bool change_direction(Snake *s, char key);

bool grow_snake(Snake *s);

#endif // !SNAKE_H
