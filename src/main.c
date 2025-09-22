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
        int history_items = 0;
        WINDOW *history = NULL;
        WINDOW *input = NULL;
        WINDOW *input_field;

        int i;
        int x, y;

        initscr();
        refresh();

        game_setup(players, &dealer, &deck, &history, &input);

        history_items++; history_clear(history, &history_items);
        history_log(history, "Game started. Type \"hit\" to hit. Anything else is a stand.\n");


        getmaxyx(input, y, x);
        input_field = derwin(input, y - 2, x - 2, 1, 1);

        dealer.chips = BUY_IN * 1000;

        for (i = 0; i < MAX_PLAYERS; i++) {
                is_player[i] = 1;
                players[i].chips = 20;
        }

        rounds = 0;
        while (rounds < 1024) {
                
                busts = 0;
                stands = 0;

                stands = dealer_ai(&dealer, &deck);
                history_items++; history_clear(history, &history_items);

                if (stands) {
                        history_log(history, "The dealer has decided to stand.\n");
                } else {
                        history_log(history, "The dealer has hit.\n");
                }

                chips_print(&dealer);
                wrefresh(dealer.status);

                for (i = 0; i < MAX_PLAYERS; i++) {
                        if (players[i].bet == 0) {
                                if (players[i].chips >= BUY_IN) {
                                        players[i].bet += BUY_IN;
                                        players[i].chips -= BUY_IN;
                                } else {
                                        history_items++; history_clear(history, &history_items);
                                        wprintw(history, "%s does not have enough for a buy-in.\n", players[i].name);
                                        wrefresh(history);
                                        players[i].lost = 1;
                                }
                        }

                        player_check(&players[i], &dealer);
                        chips_print(&players[i]);                        
                        if (players[i].lost) {
                                busts++;
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
                                history_items++; history_clear(history, &history_items);
                                wprintw(history, "%s has decided to hit.\n", players[i].name);
                                wrefresh(history);
                        } else {
                                history_items++; history_clear(history, &history_items);
                                wprintw(history, "%s has decided to stand.\n", players[i].name);
                                wrefresh(history);
                                stands++;
                        }

                        wattroff(players[i].status, A_REVERSE);
                        mvwprintw(players[i].status, 0, 1, " %s ", players[i].name);
                        chips_print(&players[i]);
                        wrefresh(players[i].status);

                        chips_print(&dealer);
                        wrefresh(dealer.status);
                }

                if (stands + busts == MAX_PLAYERS + 1) {
                        card_get(&dealer, &deck);

                        if (dealer.lbound > 21) {
                                dealer.lost = 1;
                        } else {
                                dealer.lost = 0;
                        }

                        for (; dealer.ubound > 21; dealer.ubound -= 10);

                        for (i = 0; i < MAX_PLAYERS; i++) {
                                if (!players[i].lost) {
                                        for (; players[i].ubound > 21; players[i].ubound -= 10);
                                        if (dealer.lost || players[i].ubound > dealer.ubound) {
                                                players[i].chips += players[i].bet * 2;
                                                players[i].bet = 0;
                                                dealer.chips -= players[i].bet;
                                        } else {
                                                dealer.chips += players[i].bet;
                                                players[i].bet = 0;
                                        }
                                }
                        }
                        history_items++; history_clear(history, &history_items);
                        history_log(history, "Enter \"y\" for another round and anything else to quit");
                        mvwgetnstr(input_field, 0, 0, b, 1024);
                        werase(input_field);
                        wrefresh(input_field);

                        if (b[0] != 'y') {
                                break;
                        } else {
                                game_reset(players, &dealer, &deck, &history, &input);
                        }
                }
        }

        delwin(input_field);
        game_cleanup(players, &dealer, &deck, &history, &input);
        endwin();

        return 0;
}
