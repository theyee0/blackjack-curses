#include "cards.h"

void game_setup(struct player *players, struct player *dealer) {
        int i;

        int hand_width = COLS / MAX_PLAYERS;
        int hand_height = LINES / 2;

        for (i = 0; i < MAX_PLAYERS; i++) {
                players[i].hand = newwin(hand_height, hand_width, LINES / 2, i * hand_width);
                box(players[i].hand, 0, 0);
                touchwin(players[i].hand);
                wrefresh(players[i].hand);
        }

        dealer->hand = newwin(LINES / 2, COLS / 2, 0, 0);
        dealer->status = newwin(LINES / 2, COLS / 2, 0, COLS / 2);

        box(dealer->hand, 0, 0);
        box(dealer->status, 0, 0);
        wrefresh(dealer->hand);
        wrefresh(dealer->status);
}

void print_hand(struct player *players) {
        int i, j;
        char *card;

        for (i = 0; i < MAX_PLAYERS; i++) {
                if (players[i].lost) {
                        printf("Player %d has busted.\n", i);
                }

                printf("Player %d has ", i);

                for (j = 0; j < players[i].num_cards; j++) {
                        switch(players[i].cards[j].type) {
                        case NONE:
                        break;
                        case SPADE:
                                card = "spade";
                                break;
                        case CLUB:
                                card = "club";
                                break;
                        case HEART:
                                card = "heart";
                                break;
                        case DIAMOND:
                                card = "diamond";
                                break;
                        }

                        printf("%s,%d ", card, players[i].cards[j].value);
                }

                printf("\n");
        }
}

void print_chips(struct player *players) {
        int i;

        for (i = 0; i < MAX_PLAYERS; i++) {
                werase(players[i].status);
                if (players[i].lost) {
                        wprintw(players[i].status, "%s has busted.\n", players[i].name);
                        continue;
                }

                wprintw(players[i].status, "%s: %d chips\n", players[i].name, players[i].chips);

                if (players[i].ubound == players[i].lbound) {
                        wprintw(players[i].status, "%s: %d points", players[i].name, players[i].lbound);
                } else {
                        wprintw(players[i].status, "%s: %d-%d points", players[i].name, players[i].lbound, players[i].ubound);
                }
        }
}

void print_dealer(struct player *dealer) {
        printf("%p\n", (void*)dealer);
}

void get_card(struct player *player, struct deck deck) {
        int sel = rand() % deck.remaining;
        struct card draw = deck.cards[sel];

        deck.cards[sel] = deck.cards[--deck.remaining];

        player->lbound += draw.value;
}
        

void check_player(struct player *player, struct player *dealer) {
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
