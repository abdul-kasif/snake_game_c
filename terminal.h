#ifndef TERMINAL_H
#define TERMINAL_H

#include "food.h"
#include "snake.h"

#define GRID_WIDTH 120
#define GRID_HEIGHT 30

void enable_raw_mode(void);
void disable_raw_mode(void);
int read_key(void);

void clear_terminal(void);
void clear_grid(char grid[GRID_HEIGHT][GRID_WIDTH]);
void draw_snake_on_grid(char grid[GRID_HEIGHT][GRID_WIDTH], const Snake *s);
void render_grid(const char grid[GRID_HEIGHT][GRID_WIDTH]);

void draw_food_on_grid(char grid[GRID_HEIGHT][GRID_WIDTH], const Food *f);

#endif // !TERMINAL_H
