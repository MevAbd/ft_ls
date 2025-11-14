NAME = ft_ls

CC = gcc
CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = obj

SRCS = main.c libft.c ft_recursive.c ft_sort.c ft_entries.c ft_option.c ft_parse.c ft_operands.c ft_display.c ft_format.c ft_get_info.c ft_print.c ft_free.c ft_align.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c ft_ls.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
