#include "tui.h"

void print_catalog(WINDOW* win, infoCatalog** inf_catalogList,
                   auxiliaryStructure* auxStr, int current_catalog) {
  int startx = 1, starty = 1;
  for (int i = 1; i <= auxStr->size[current_catalog]; ++i) {
    wattroff(win, A_DIM);
    if ((i - 1) == auxStr->current_index[current_catalog] &&
        current_catalog == auxStr->current_catalog) {
      wattron(win, COLOR_PAIR(2));
    } else if (inf_catalogList[i - 1]->status == 0) {
      wattron(win, COLOR_PAIR(1));
    } else if (inf_catalogList[i - 1]->status == 1) {
      wattron(win, COLOR_PAIR(3) | A_BOLD);
    } else {
      wattron(win, A_DIM);
    }

    mvwprintw(win, starty + i, startx, "%s", inf_catalogList[i - 1]->name);

    wattroff(win, A_DIM);
    wattron(win, A_BOLD);
    mvwaddch(win, starty + i, startx + 66, ACS_VLINE);
    if (i == 1) {
      mvwprintw(win, starty + i, startx + 67, "---UP---");
    } else {
      mvwprintw(win, starty + i, startx + 67, "%d",
                inf_catalogList[i - 1]->size);
    }
    mvwaddch(win, starty + i, startx + 75, ACS_VLINE);
    mvwprintw(win, starty + i, startx + 76, "%s",
              inf_catalogList[i - 1]->edit_time);
    wattroff(win, A_BOLD);
    wattrset(win, COLOR_PAIR(1) | A_DIM);
  }
  wrefresh(win);
}

void init_wins(WINDOW** wins, int n, infoCatalog*** inf_catalogList,
               auxiliaryStructure* auxStr) {
  int y = 1, x = 1;
  for (int i = 0, j = 1; i < n; ++i, ++j) {
    wins[i] = newwin(NLINES, NCOLS, y, x);
    box(wins[i], 0, 0);
    wbkgd(wins[i], COLOR_PAIR(1));
    x += NCOLS;

    print_catalog(wins[i], inf_catalogList[i], auxStr, i);
    // mvwprintw(wins[i], 1, 1, "%s", (*inf_catalogList)[0]->name);
    // wrefresh(wins[i]);
  }
}

int main_window(infoCatalog*** inf_catalogList, auxiliaryStructure* auxStr,
                path_t** Path_catalog) {
  WINDOW* wins[2];
  PANEL* panels[2];
  PANEL* active_panel;

  wbkgd(stdscr, COLOR_PAIR(1));
  init_wins(wins, 2, inf_catalogList, auxStr);

  panels[0] = new_panel(wins[0]);
  panels[1] = new_panel(wins[1]);

  set_panel_userptr(panels[0], panels[1]);
  set_panel_userptr(panels[1], panels[0]);

  top_panel(panels[0]);

  update_panels();
  doupdate();

  int ch, flag = 1;
  active_panel = panels[0];

  while (flag) {
    ch = getch();
    int Cur_cat = auxStr->current_catalog;
    switch (ch) {
      case 9:
        int temp = auxStr->current_index[Cur_cat];
        auxStr->current_index[Cur_cat] = auxStr->size[Cur_cat] + 1;
        print_catalog(wins[Cur_cat], inf_catalogList[Cur_cat], auxStr, Cur_cat);
        auxStr->current_index[Cur_cat] = temp;
        active_panel = (PANEL*)panel_userptr(active_panel);
        top_panel(active_panel);
        auxStr->current_catalog = Cur_cat = (Cur_cat == 0) ? 1 : 0;
        break;
      case KEY_DOWN:
        auxStr->current_index[Cur_cat] =
            (auxStr->current_index[Cur_cat] + auxStr->size[Cur_cat] + 1) %
            auxStr->size[Cur_cat];
        break;
      case KEY_UP:
        auxStr->current_index[Cur_cat] =
            (auxStr->current_index[Cur_cat] + auxStr->size[Cur_cat] - 1) %
            auxStr->size[Cur_cat];
        break;
      case '\n':
        if (inf_catalogList[Cur_cat][auxStr->current_index[Cur_cat]]->status ==
            0) {
          changePathCatalog(
              inf_catalogList[Cur_cat][auxStr->current_index[Cur_cat]]->name,
              Path_catalog[Cur_cat]);
          flag = 0;
        }
        break;
      case KEY_F(1):
        flag = 0;
        auxStr->run = 0;
        break;
    }
    print_catalog(wins[Cur_cat], inf_catalogList[Cur_cat], auxStr, Cur_cat);
    update_panels();
    doupdate();
  }

  return 0;
}