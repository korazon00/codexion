NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC = codexion.c coders.c dongles.c ft_time.c heap.c monitor.c parsing.c

OBJ = $(SRC:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c codexion.h 
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all