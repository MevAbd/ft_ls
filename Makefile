# Makefile pour ft_ls

NAME = ft_ls

# Compilateur et flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Dossier pour les fichiers objets
OBJ_DIR = obj

# Fichiers sources
SRCS = main.c ft_strings.c ft_files.c ft_args.c ft_operands.c ft_output.c ft_display.c ft_format.c ft_get_info.c

# Fichiers objets (générés automatiquement dans OBJ_DIR)
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Règle par défaut
all: $(OBJ_DIR) $(NAME)

# Création du dossier obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Création de l'exécutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Compilation des fichiers objets dans le dossier obj
$(OBJ_DIR)/%.o: %.c ft_ls.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -rf $(OBJ_DIR)

# Nettoyage complet (objets + exécutable)
fclean: clean
	rm -f $(NAME)

# Recompilation complète
re: fclean all

# Phony targets (ne sont pas des fichiers)
.PHONY: all clean fclean re

