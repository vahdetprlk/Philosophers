NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread -pthread

SRC=	src/ft_atoi.c \
		src/ft_calloc.c \
 		src/philo.c

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
