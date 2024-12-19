NAME = minitalk.a

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = $(wildcard *c)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *o

fclean: clean
	rm -f $(NAME)

re: fclean all
