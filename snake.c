#include "snake.h"
#include "terminal.h"
#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Snake create_snake(void) {
  Snake s = {0};
  s.length = INITIAL_SNAKE_LENGTH;

  s.body = malloc(sizeof(Position) * s.length);
  if (!s.body) {
    perror("malloc allocation failed");
    exit(EXIT_FAILURE);
  }

  s.body[0] = (Position){5, 5};
  s.body[1] = (Position){4, 5};
  s.body[2] = (Position){3, 5};

  s.dx = 1;
  s.dy = 0;

  return s;
}

void destroy_snake(Snake *s) {
  if (s && s->body) {
    free(s->body);
    s->body = NULL;
    s->length = 0;
  }
}

void move_snake(Snake *s) {
  if (!s || s->length <= 0) {
    return;
  }

  Position new_head = {.x = s->body[0].x + s->dx, .y = s->body[0].y + s->dy};

  for (int i = s->length - 1; i > 0; i--) {
    s->body[i] = s->body[i - 1];
  }

  s->body[0] = new_head;
}

bool change_direction(Snake *s, char key) {

  if (!s) {
    return false;
  }

  int new_dx = s->dx;
  int new_dy = s->dy;

  switch (key) {
  case 'w':
  case 'W':
    if (s->dy == 0) {
      new_dx = 0;
      new_dy = -1;
    }
    break;
  case 's':
  case 'S':
    if (s->dy == 0) {
      new_dx = 0;
      new_dy = 1;
    }
    break;
  case 'a':
  case 'A':
    if (s->dx == 0) {
      new_dx = -1;
      new_dy = 0;
    }
    break;
  case 'd':
  case 'D':
    if (s->dx == 0) {
      new_dx = 1;
      new_dy = 0;
    }
    break;
  default:
    return false;
  }

  if (new_dx != s->dx || new_dy != s->dy) {
    s->dx = new_dx;
    s->dy = new_dy;
    return true;
  }

  return false;
}

bool grow_snake(Snake *s) {
  if (!s)
    return false;

  Position *new_body = realloc(s->body, sizeof(Position) * (s->length + 1));

  if (!new_body) {
    return false;
  }

  s->body = new_body;
  s->body[s->length] = s->body[s->length - 1];
  s->length++;

  return true;
}

bool snake_hit_wall(const Snake *s) {
  if (!s || s->length <= 0) {
    return false;
  }

  int x = s->body[0].x;
  int y = s->body[0].y;

  return (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT);
}

bool snake_hit_self(const Snake *s) {
  if (!s || s->length <= 0) {
    return false;
  }

  int head_x = s->body[0].x;
  int head_y = s->body[0].y;

  for (int i = 1; i < s->length; i++) {
    if (s->body[i].x == head_x && s->body[i].y == head_y) {
      return true;
    }
  }

  return false;
}
