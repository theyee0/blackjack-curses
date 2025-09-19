#ifndef __CARDS_H
#define __CARDS_H

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <panel.h>

#define MAX_PLAYERS 5

enum card_type {
        NONE,
        SPADE,
        CLUB,
        HEART,
        DIAMOND
};

struct card {
        enum card_type type;
        PANEL *display;
        int value;
};

struct deck {
        struct card *cards;
        int remaining;
};

struct player {
        int ubound;
        int lbound;
        int bet;
        int chips;
        int lost;
        char *name;
        WINDOW *hand;
        WINDOW *status;
        int num_cards;
        struct card *cards;
};

struct window {
        int width;
        int height;
};

void game_setup(struct player *players, struct player *dealer);

void print_hand(struct player *players);

void print_chips(struct player *players);

void print_dealer(struct player *players);

void get_card(struct player *player, struct deck deck);

void check_player(struct player *player, struct player *dealer);
#endif
