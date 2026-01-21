#include "food.h"
#include "snake.h"
#include "terminal.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TICK_INTERVEL_MS 150

typedef enum { GAME_RUNNING, GAME_OVER } GameState;

static void reset_game(Snake *snake, Food *food, int *score) {
  destroy_snake(snake);
  *snake = create_snake();
  *food = spawn_food();
  *score = 0;
}

int main(void) {
  enable_raw_mode();

  bool running = true;

  GameState state = GAME_RUNNING;

  Snake snake = create_snake();
  Food food = spawn_food();

  char grid[GRID_HEIGHT][GRID_WIDTH];
  int score = 0;

  srand((unsigned int)time(NULL));

  while (running) {

    while (state == GAME_RUNNING) {
      int key = read_key();

      if (key == 'q' || key == 'Q') {
        break;
      }

      if (key != -1) {
        change_direction(&snake, key);
      }

      move_snake(&snake);

      if (snake_hit_wall(&snake)) {
        state = GAME_OVER;
        break;
      }

      if (snake_hit_self(&snake)) {
        state = GAME_OVER;
        break;
      }

      if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        grow_snake(&snake);
        food = spawn_food();
        score++;
      }

      clear_grid(grid);
      draw_snake_on_grid(grid, &snake);
      draw_food_on_grid(grid, &food);
      clear_terminal();
      render_score(score);
      render_grid(grid);

      usleep(TICK_INTERVEL_MS * 1000);
    }

    render_game_over_with_score(score);

    while (1) {
      int key = read_key();

      if (key == 'r' || key == 'R') {
        reset_game(&snake, &food, &score);
        state = GAME_RUNNING;
        break;
      }

      if (key == 'q' || key == 'Q') {
        running = false;
        break;
      }
    }

    usleep(TICK_INTERVEL_MS * 100);
  }
  destroy_snake(&snake);
  disable_raw_mode();

  return 0;
}
