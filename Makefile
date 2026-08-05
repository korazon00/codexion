NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = project/codexion.c project/coders.c project/dongles.c project/ft_time.c project/heap.c project/monitor.c project/parsing.c project/cdr_rtn_func.c project/helpful_functs.c project/random_func.c

OBJ = $(SRC:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c project/codexion.h
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all