#include "food.h"
#include "snake.h"
#include "terminal.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TICK_INTERVEL_MS 150

int main(void) {
  enable_raw_mode();

  Snake snake = create_snake();
  Food food = spawn_food();
  char grid[GRID_HEIGHT][GRID_WIDTH];

  srand((unsigned int)time(NULL));

  while (1) {
    int key = read_key();

    if (key == 'q' || key == 'Q') {
      break;
    }

    if (key != -1) {
      change_direction(&snake, key);
    }

    move_snake(&snake);

    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
      grow_snake(&snake);
      food = spawn_food();
    }

    clear_grid(grid);
    draw_snake_on_grid(grid, &snake);
    draw_food_on_grid(grid, &food);
    render_grid(grid);

    usleep(TICK_INTERVEL_MS * 1000);
  }

  destroy_snake(&snake);
  disable_raw_mode();

  return 0;
}
