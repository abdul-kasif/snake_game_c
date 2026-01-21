#include "food.h"
#include "snake.h"
#include "terminal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SCREEN_WAIT_TIME 400000

typedef enum { GAME_RUNNING, GAME_PAUSED, GAME_OVER } GameState;

bool snake_occupies(const Snake *s, int x, int y) {
  if (!s || s->length <= 0) {
    return false;
  }

  for (int i = 0; i < s->length; i++) {
    if (s->body[i].x == x && s->body[i].y == y) {
      return true;
    }
  }

  return false;
}

static Food spawn_food_safe(const Snake *snake) {
  Food food;

  do {
    food = spawn_food();
  } while (snake_occupies(snake, food.x, food.y));

  return food;
}

static int compute_tick_delay_ms(const int score) {
  const int base_delay = 150;
  const int min_delay = 50;
  const int step_score = 5;
  const int step_delay = 10;

  int steps = score / step_score;

  int delay = base_delay - steps * step_delay;

  if (delay < min_delay) {
    delay = min_delay;
  }

  return delay;
}

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
  Food food = spawn_food_safe(&snake);

  char grid[GRID_HEIGHT][GRID_WIDTH];
  int score = 0;

  srand((unsigned int)time(NULL));

  while (running) {

    while (state != GAME_OVER) {
      int key = read_key();

      if (key == 'q' || key == 'Q') {
        break;
      }

      if (key != -1) {
        change_direction(&snake, key);
      }

      if (key == 'p' || key == 'P') {
        if (state == GAME_RUNNING) {
          printf("I am changing this;");
          state = GAME_PAUSED;
        } else if (state == GAME_PAUSED) {
          state = GAME_RUNNING;
        }
      }

      if (state == GAME_RUNNING) {
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
          food = spawn_food_safe(&snake);
          score++;
        }
      }

      clear_grid(grid);
      draw_snake_on_grid(grid, &snake);
      draw_food_on_grid(grid, &food);
      clear_terminal();
      render_score(score);
      render_grid(grid);
      if (state == GAME_PAUSED) {
        render_paused();
        usleep(10000);
        continue;
      }

      int delay_ms = compute_tick_delay_ms(score);
      usleep(delay_ms * 1000);
    }

    usleep(SCREEN_WAIT_TIME);
    clear_terminal();
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

    usleep(10000);
  }
  destroy_snake(&snake);
  disable_raw_mode();

  return 0;
}
