CC=gcc
SRC=src/main.c\
	src/time.c\
	src/forks.c\
	src/philo.c\
	src/utils.c\
	src/parse_arg.c\
	src/string_utils.c
INCLUDE=src/time.h\
	src/forks.h\
	src/philo.h\
	src/utils.h\
	src/parse_arg.h\
	src/types.h\
	src/string_utils.h

DEPS= $(SRC) $(INCLUDE)
CFLAGS=-Wall -Wextra -Werror
DFLAGS=-g -fsanitize=thread
NAME=philo

all: $(NAME)

$(NAME): $(DEPS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

re: clean all

run: all
	./philo

test:
	echo "no test specified"

.PHONY: all clean fclean re test run
