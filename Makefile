NAME = ft_ping
FLAGS = -Wall -Wextra -Werror
SRCS = ft_ping.c utils.c

all: $(NAME)

$(NAME): $(SRCS)
	@gcc $(FLAGS) $(SRCS) -o $(NAME)
	@echo "Hi Bro."

clean:
	@rm -rf $(NAME)

fclean: clean

re: clean all