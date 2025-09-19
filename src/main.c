#include <ncurses.h>
#include <unistd.h>
#include <getopt.h>

#include "cards.h"

int main(void) {
        struct player players[MAX_PLAYERS];
        struct player dealer;

        initscr();

        game_setup(players, &dealer);

        /*
        while (running) {
                print_hand(players, win);
                print_chips(players, win);
                print_dealer(players, win);
                refresh();
        }
        */
        getchar();
        endwin();

        printf("%d %d\n", COLS, LINES);
        return 0;
}
