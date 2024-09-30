# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/26 10:12:54 by amoubine          #+#    #+#              #
#    Updated: 2024/09/30 20:28:18 by asebaai          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wextra -Werror -Wall -fsanitize=address -g3

INCLUDE = -lreadline

SOURCES = token.c split.c init_token.c add_tokenv2.c add_token.c utils.c

OBJ = $(SOURCES:.c=.o)

ALL = ${NAME}

$(NAME) : $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(INCLUDE)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean :
	rm -rf $(OBJ)

fclean : clean
		rm -rf $(NAME) $(OBJ)

re : fclean all

