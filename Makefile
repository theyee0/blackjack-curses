NAME := blackjack_curses

CC := gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -ansi

INC_DIR := inc
SRC_DIR := src
OBJ_DIR := obj

SRC_FILES := main cards
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SRC_FILES)))

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -I $(INC_DIR) $(OBJ_FILES) -o $(NAME) -lncurses
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -I $(INC_DIR) $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: all clean debug release profile

clean:
	-rm -rf $(OBJ_DIR)
	-rm $(NAME)

debug: CFLAGS += -g -fsanitize=address
debug: $(NAME)

release: CFLAGS += -O3 -flto
release: $(NAME)

profile: CFLAGS += -pg
profile: $(NAME)
