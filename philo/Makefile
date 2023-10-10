NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread -pthread

SRC=	src/ft_atoi.c \
		src/ft_calloc.c \
		src/ft_life_cycle.c \
		src/ft_mutex_check.c \
		src/ft_philo_create.c \
		src/ft_time.c \
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
