#include "cards.h"

static int card_width;
static int card_height;

void game_reset(struct player *players, struct player *dealer, struct deck *deck, WINDOW **history, WINDOW **input) {
        int i;

        for (i = 0; i < MAX_PLAYERS; i++) {
                player_reset(&players[i]);
        }

        player_reset(dealer);

        werase(*history);
        box(*history, 0, 0);
        wrefresh(*history);

        werase(*input);
        box(*input, 0, 0);
        wrefresh(*input);

        deck_reset(deck);
}

int dealer_ai(struct player *dealer, struct deck *deck) {
        if (dealer->lbound >= 10) {
                return 1;
        } else {
                card_get(dealer, deck);
                return 0;
        }
}

void game_cleanup(struct player *players, struct player *dealer, struct deck *deck, WINDOW **history, WINDOW **input) {
        int i;

        for (i = 0; i < MAX_PLAYERS; i++) {
                player_cleanup(&players[i]);
        }

        player_cleanup(dealer);

        delwin(*history);
        delwin(*input);

        deck_cleanup(deck);
}
        

void game_setup(struct player *players, struct player *dealer, struct deck *deck, WINDOW **history, WINDOW **input) {
        int i;

        int default_name_size = 16;

        int hand_width = COLS / MAX_PLAYERS;

        srand(time(NULL));

        for (i = 0; i < MAX_PLAYERS; i++) {
                players[i].status = newwin(4, hand_width, LINES - 4, i * hand_width);

                players[i].hand = newwin(LINES / 2 - 4, hand_width, LINES / 2, i * hand_width);
                players[i].cards = malloc(MAX_CARDS * sizeof(*players[i].cards));

                players[i].name = malloc(default_name_size * sizeof(*players[i].name));

                sprintf(players[i].name, "Player %d", i + 1);

                players[i].bet = 0;
                players[i].chips = BUY_IN;
                players[i].lost = 0;

                players[i].num_cards = 0;

        }

        dealer->hand = newwin(LINES / 2 - 4, COLS * 2 / 3, 0, 0);
        dealer->status = newwin(4, COLS * 2 / 3, LINES / 2 - 4, 0);
        dealer->cards = malloc(MAX_CARDS * sizeof(*dealer->cards));
        dealer->bet = 0;

        dealer->name = malloc(default_name_size * sizeof(*dealer->name));
        sprintf(dealer->name, " Dealer ");

        deck->cards = malloc(NUM_DECKS * DECK_SIZE * sizeof(*deck->cards));

        *history = newwin(LINES / 2 - 4, hand_width * MAX_PLAYERS - COLS * 2 / 3, 0, COLS * 2 / 3);
        *input = newwin(4, hand_width * MAX_PLAYERS - COLS * 2 / 3, LINES / 2 - 4, COLS * 2 / 3);

        game_reset(players, dealer, deck, history, input);
        assert(deck->remaining > 0);

        getmaxyx(players[0].hand, card_height, card_width);
        card_height -= 2;
        card_width -= 2;
}

void chips_print(struct player *players) {
        werase(players->status);
        box(players->status, 0, 0);
        mvwprintw(players->status, 0, 1, " %s ", players->name);

        if (players->lost) {
                mvwprintw(players->status, 1, 1, "%s has busted.\n", players->name);
                wrefresh(players->status);
                return;
        }

        mvwprintw(players->status, 1, 1, "%d chips remain.\n", players->chips);

        if (players->bet > 0) {
                mvwprintw(players->status, 2, 1, "%d chips betted.\n", players->bet);
        }

        if (players->ubound == players->lbound) {
                mvwprintw(players->status, 3, 1, "%d points.", players->lbound);
        } else {
                mvwprintw(players->status, 3, 1, "%d-%d points.", players->lbound, players->ubound);
        }
}

WINDOW *card_draw(struct player *player, struct card draw) {
        WINDOW *card;
        char *card_types[5] = {"Unknown Type", "Spades", "Clubs", "Hearts", "Diamonds"};
        char *card_values[14] = {
                "NULL", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
                "Nine", "Ten", "Jack", "Queen", "King"
        };


        card = derwin(player->hand, card_height - 9, card_width - 1, player->num_cards, player->num_cards % 2 + 1);
        werase(card);

        assert(card_width - 1 > 0 && card_height - 10 > 0);

        box(card, 0, 0);

        mvwprintw(card, 0, 1, "%s of %s", card_values[draw.value], card_types[draw.type]);

        return card;
}

void history_log(WINDOW *history, char *b) {
        wprintw(history, b);
        wrefresh(history);
}

void history_clear(WINDOW *history, int *history_items) {
        int y, x;

        getmaxyx(history, y, x);

        if (*history_items >= y) {
                werase(history);
                *history_items = 0*x;
        }

        wrefresh(history);

}

void card_get(struct player *player, struct deck *deck) {
        int sel;
        struct card draw;
        WINDOW *card;

        assert(deck->remaining > 0);

        sel = rand() % deck->remaining;
        draw = deck->cards[sel];

        player->num_cards++;

        card = card_draw(player, draw);

        wrefresh(card);
        wrefresh(player->hand);

        assert(player->num_cards < MAX_CARDS);
        player->cards[player->num_cards] = deck->cards[sel];
        player->cards[player->num_cards].display = card;
        deck->cards[sel] = deck->cards[--deck->remaining];

        player->lbound += (draw.value > 10) ? 10 : draw.value;

        if (draw.value == 1) {
                player->ubound += 11;
        } else {
                player->ubound += (draw.value > 10) ? 10 : draw.value;
        }
}
        

void player_check(struct player *player, struct player *dealer) {
        if (player->lbound > 21) {
                /*adjust();*/
                /* bust! */
                player->lost = 1;
                dealer->chips += player->bet;
                player->bet = 0;
        } else if (player->lbound > 15) {
                /* Getting close... */
        } else if (player->lbound > 9) {
                /* Still a ways to go */
        } else {
                /* Still low. Keep drawing! */
        }
}

void player_reset(struct player *player) {
        player->ubound = player->lbound = 0;
        player->lost = 0;
        player->num_cards = 0;
        werase(player->hand);
        box(player->hand, 0, 0);
        wrefresh(player->hand);

        werase(player->status);
        box(player->status, 0, 0);
        mvwprintw(player->status, 0, 1, " %s ", player->name);
        wrefresh(player->status);
}

void player_cleanup(struct player *player) {
        delwin(player->hand);
        delwin(player->status);
        free(player->name);
        free(player->cards);
}

void deck_reset(struct deck *deck) {
        struct card new_card;
        int i, j, k;

        deck->remaining = 0;

        for (i = 1; i <= 13; i++) {
                for (j = 1; j <= 4; j++) {
                        for (k = 0; k < NUM_DECKS; k++) {
                                new_card.value = i;
                                new_card.type = j;
                                assert(deck->remaining < NUM_DECKS * DECK_SIZE);
                                deck->cards[deck->remaining++] = new_card;
                        }
                }
        }
}

void deck_cleanup(struct deck *deck) {
        free(deck->cards);
        deck->remaining = 0;
}
