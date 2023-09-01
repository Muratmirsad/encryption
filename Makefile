NAME = encoder
CFLAGS = -Wall -Werror -Wextra
SRCS = encoder.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c

all : $(NAME)

$(NAME): $(SRCS)
	@echo compiling...
	@gcc $(CFLAGS) $(SRCS) -o $(NAME)
	@clear
	@echo " ---> philo <---"
	@echo program created.

clean:
	@rm -rf *.dSYM

fclean: clean
	@rm -rf $(NAME)
	@echo program deleted.

re: fclean all

.PHONY: clean fclean re