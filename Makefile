CC=gcc
SRC=src/main.c src/parse_arg.c
INCLUDE=include/*.h
DEPS= $(SRC) $(INCLUDE)
CFLAGS=-Wall -Wextra -Werror
DFLAGS=-g -fsanitize=address
NAME=philo

all: $(NAME)

$(NAME): $(DEPS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

re: clean all

run: all
	./philo

test:
	echo "no test specified"

.PHONY: all clean fclean re test run
