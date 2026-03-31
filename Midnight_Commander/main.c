#include <stdio.h>

#include "header.h"

int main() {
  init_curses();
  run();
  endwin();

  return 0;
}