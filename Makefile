# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/26 10:12:54 by amoubine          #+#    #+#              #
#    Updated: 2024/12/04 10:37:12 by asebaai          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wextra -Werror -Wall -g3 #-fsanitize=address 
INCLUDE = -lreadline

# Directories
SRC_DIR_E = execution
BIN_DIR = bin
LIBFT_DIR = libft

# Source files
EXEC_SRCS = token.c builtin.c builtin1.c builtin2.c builtin3.c builtin_utils.c \
		exec.c exec1.c pipes.c pipe1.c pipe_utils.c redirection.c \
		exec_utils.c exec_utiles.c builtin4.c redirection_utils.c utiles.c \
		signals.c token_utils.c token_utils1.c token_utils2.c token_utils3.c \
		token_utils4.c token_utils5.c token_utils6.c token_utils7.c token_utils8.c \
		token_utils9.c token_utils10.c token_utils11.c token_utils12.c heredoc.c \
		heredoc_v2.c token_utils13.c token_utils14.c 

# Create full paths for source and object files
SRCS = $(addprefix $(SRC_DIR_E)/, $(EXEC_SRCS))
OBJS = $(SRCS:%.c=$(BIN_DIR)/%.o)

# Make sure directories exist
DIRS = $(BIN_DIR) $(BIN_DIR)/$(SRC_DIR_E)

# Main target
all: $(NAME)

# Create necessary directories
$(BIN_DIR):
	@mkdir -p $(DIRS)

# Compile libft
$(LIBFT_DIR)/libft.a:
	@echo "\033[0;34mCompiling libft... 📚\033[0m"
	@$(MAKE) -C $(LIBFT_DIR) all bonus -s

# Compile object files
$(BIN_DIR)/%.o: %.c | $(BIN_DIR)
	@echo "\033[0;32mCompiling $<... 🔨\033[0m"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Link everything together
$(NAME): $(BIN_DIR) $(OBJS) $(LIBFT_DIR)/libft.a
	@echo "\033[0;32mLinking minishell... 🔗\033[0m"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_DIR)/libft.a -o $(NAME) $(INCLUDE)
	@echo "\033[0;32mBuild complete! ✨\033[0m"

# Clean rules
clean:
	@echo "\033[0;31mCleaning object files... 🧹\033[0m"
	@rm -rf $(BIN_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean -s

fclean: clean
	@echo "\033[0;31mCleaning everything... 🗑️\033[0m"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean -s

re: fclean all

.PHONY: all clean fclean re