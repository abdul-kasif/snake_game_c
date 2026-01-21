#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios old_termios;

void enable_raw_mode() {
  struct termios raw;

  tcgetattr(STDIN_FILENO, &old_termios);
  atexit(disable_raw_mode);

  raw = old_termios;
  raw.c_lflag &= ~(ICANON | ECHO);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void disable_raw_mode() { tcsetattr(STDIN_FILENO, TCSANOW, &old_termios); }

int read_key(void) {
  char c;
  ssize_t n = read(STDIN_FILENO, &c, 1);
  return (n > 0) ? (unsigned char)c : -1;
}

void clear_terminal(void) {
  printf("\033c");
  fflush(stdout);
}

void clear_grid(char grid[GRID_HEIGHT][GRID_WIDTH]) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    memset(grid[y], '.', GRID_WIDTH);
  }
}

void draw_snake_on_grid(char grid[GRID_HEIGHT][GRID_WIDTH], const Snake *s) {
  if (!s) {
    return;
  }

  for (int i = 0; i < s->length; i++) {
    int x = s->body[i].x;
    int y = s->body[i].y;

    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
      grid[y][x] = (i == 0) ? '0' : 'o';
    }
  }
}

void render_grid(const char grid[GRID_HEIGHT][GRID_WIDTH]) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      putchar(grid[y][x]);
    }
    putchar('\n');
  }
}

void draw_food_on_grid(char grid[GRID_HEIGHT][GRID_WIDTH], const Food *f) {
  if (!f) {
    return;
  }

  int x = f->x;
  int y = f->y;

  if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
    grid[y][x] = '#';
  }
}

void render_score(const int score) { printf("Score: %d\n\n", score); }

void render_game_over_with_score(const int score) {
  usleep(500 * 1000);
  clear_terminal();
  printf("\n\n");
  printf("   =====================\n");
  printf("        GAME OVER\n");
  printf("   =====================\n");
  printf("\n   Final Score: %d\n", score);
  printf("\n   Press 'r' to restart\n");
  printf("\n   Press 'q' to quit\n");
  fflush(stdout);
}
