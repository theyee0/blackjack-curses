#ifndef __CARDS_H
#define __CARDS_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define MAX_PLAYERS 5
#define NUM_DECKS 8
#define DECK_SIZE 52
#define MAX_CARDS 21
#define BUY_IN 10

enum card_type {
        UNKNOWN,
        SPADE,
        CLUB,
        HEART,
        DIAMOND
};

struct card {
        enum card_type type;
        WINDOW *display;
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

void history_log(WINDOW *history, char *b);
void history_clear(WINDOW *history, int *history_items);

void game_reset(struct player *players, struct player *dealer, struct deck *deck, WINDOW **history, WINDOW **input);

void game_cleanup(struct player *players, struct player *dealer, struct deck *deck, WINDOW **history, WINDOW **input);

void game_setup(struct player *players, struct player *dealer, struct deck *deck, WINDOW **history, WINDOW **input);

void hand_print(struct player *player);

void chips_print(struct player *player);

int dealer_ai(struct player *dealer, struct deck *deck);

void card_get(struct player *player, struct deck *deck);

void player_check(struct player *player, struct player *dealer);

void player_reset(struct player *player);

void player_cleanup(struct player *player);

void deck_reset(struct deck *deck);

void deck_cleanup(struct deck *deck);
#endif
