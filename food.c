#include "food.h"
#include "terminal.h"
#include <stdlib.h>

Food spawn_food(void) {
  Food f;
  f.x = rand() % GRID_WIDTH;
  f.y = rand() % GRID_HEIGHT;
  return f;
}
