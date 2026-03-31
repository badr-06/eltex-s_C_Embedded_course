#include "tui.h"

void init_curses() {
  initscr();
  start_color();
  // curs_set(0);
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_CYAN);
  init_pair(3, COLOR_GREEN, COLOR_BLUE);
  // init_pair(3, COLOR_WHITE, COLOR_BLUE);
}