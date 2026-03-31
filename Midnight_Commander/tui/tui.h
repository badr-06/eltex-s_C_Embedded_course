#ifndef TUI_H
#define TUI_H

#include <panel.h>

#include "../lib.h"

int main_window(infoCatalog*** inf_catalogList, auxiliaryStructure* auxStr,
                path_t** Path_catalog);
void init_curses();

#endif