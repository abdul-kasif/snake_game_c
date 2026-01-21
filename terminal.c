#include "terminal.h"
#include <asm-generic/ioctls.h>
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

TerminalSize get_terminal_size(void) {
  struct winsize ws;
  TerminalSize size = {0, 0};

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == 0) {
    size.cols = ws.ws_col;
    size.rows = ws.ws_row;
  }

  return size;
}

static void compute_offsets(int *left, int *top) {
  TerminalSize ts = get_terminal_size();

  *left = (ts.cols - GRID_WIDTH) / 2;
  *top = (ts.rows - GRID_HEIGHT) / 2;

  if (*left < 0)
    *left = 0;
  if (*top < 0)
    *top = 0;
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
  int left_pad, top_pad;
  compute_offsets(&left_pad, &top_pad);

  for (int i = 0; i < top_pad; i++) {
    putchar('\n');
  }

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int i = 0; i < left_pad; i++) {
      putchar(' ');
    }

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

static void print_text_center(const char *text, const TerminalSize *ts,
                              const int num_newlines) {
  int left = (ts->cols - (int)strlen(text)) / 2;
  if (left < 0) {
    left = 0;
  }
  for (int s = 0; s < left; s++) {
    putchar(' ');
  }
  printf("%s", text);

  for (int i = 0; i < num_newlines; i++) {
    putchar('\n');
  }
}

void render_score(const int score) {
  TerminalSize ts = get_terminal_size();
  char buffer[64];

  snprintf(buffer, sizeof(buffer), "Score: %d", score);
  print_text_center(buffer, &ts, 2);
}

void render_game_over_with_score(const int score) {
  TerminalSize ts = get_terminal_size();

  int top = ts.rows / 2 - 4;
  if (top < 0)
    top = 0;

  for (int i = 0; i < top; i++)
    putchar('\n');

  const char *lines[] = {"=====================", "     GAME OVER       ",
                         "=====================", "", NULL};

  for (int i = 0; lines[i]; i++) {
    print_text_center(lines[i], &ts, 1);
  }

  char score_line[64];
  snprintf(score_line, sizeof(score_line), "Final Score: %d", score);
  print_text_center(score_line, &ts, 2);

  const char *msg = "Press 'r' to restart | Press 'q' to quit";
  print_text_center(msg, &ts, 2);
}

void render_paused(void) {
  TerminalSize ts = get_terminal_size();

  printf("\n");
  const char *lines[] = {"===== PAUSED =====", "Press 'p' to resume", NULL};
  for (int i = 0; lines[i]; i++) {
    print_text_center(lines[i], &ts, 1);
  }
}
