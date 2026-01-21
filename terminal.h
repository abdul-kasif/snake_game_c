#ifndef TERMINAL_H
#define TERMINAL_H

#include "food.h"
#include "snake.h"

#define GRID_WIDTH 80
#define GRID_HEIGHT 20

void enable_raw_mode(void);
void disable_raw_mode(void);
int read_key(void);

void clear_terminal(void);
void clear_grid(char grid[GRID_HEIGHT][GRID_WIDTH]);
void draw_snake_on_grid(char grid[GRID_HEIGHT][GRID_WIDTH], const Snake *s);
void render_grid(const char grid[GRID_HEIGHT][GRID_WIDTH]);

void draw_food_on_grid(char grid[GRID_HEIGHT][GRID_WIDTH], const Food *f);

void render_score(const int score);
void render_game_over_with_score(const int score);

void render_paused(void);

#endif // !TERMINAL_H
