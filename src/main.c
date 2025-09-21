#include <ncurses.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>

#include "cards.h"

int main(void) {
        char b[1024];
        int is_player[MAX_PLAYERS];
        int stands;
        int busts;
        int rounds;
        struct player players[MAX_PLAYERS];
        struct player dealer;
        struct deck deck;
        WINDOW *history = NULL;
        WINDOW *input = NULL;
        WINDOW *input_field;

        int i;
        int x, y;

        initscr();
        refresh();

        game_setup(players, &dealer, &deck, &history, &input);

        getmaxyx(input, y, x);
        input_field = derwin(input, y - 2, x - 2, 1, 1);

        for (i = 0; i < MAX_PLAYERS; i++) {
                is_player[i] = 1;
        }

        rounds = 0;
        while (rounds < 1024) {
                busts = 0;
                stands = 0;

                for (i = 0; i < MAX_PLAYERS; i++) {
                        player_check(&players[i], &dealer);
                        if (players[i].lost) {
                                busts++;
                                chips_print(&players[i]);
                                continue;
                        }

                        if (!is_player[i]) {
                                continue;
                        }

                        wattron(players[i].status, A_REVERSE);
                        mvwprintw(players[i].status, 0, 1, " %s ", players[i].name);
                        wrefresh(players[i].status);

                        mvwgetnstr(input_field, 0, 0, b, 1024);
                        werase(input_field);
                        wrefresh(input_field);

                        if (!strcmp(b, "hit")) {
                                card_get(&players[i], &deck);
                                player_check(&players[i], &dealer);
                        } else {
                                stands++;
                        }

                        wattroff(players[i].status, A_REVERSE);
                        wrefresh(players[i].status);
                                                
                                
                        chips_print(&players[i]);
                }

                if (stands + busts == MAX_PLAYERS) {
                        break;
                }
        }

        delwin(input_field);
        game_cleanup(players, &dealer, &deck, &history, &input);
        endwin();

        return 0;
}
