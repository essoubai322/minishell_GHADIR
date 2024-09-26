# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amoubine <amoubine@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/26 10:12:54 by amoubine          #+#    #+#              #
#    Updated: 2024/09/26 10:39:23 by amoubine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

FLAGS = -Wextra -Werror -Wall

INCLUDE = -lreadline

SOURCES = token.c split.c

OBJ = $(SOURCES:.c=.o)

ALL = ${NAME}

$(NAME) : $(OBJ)
		$(CC) $(OBJ) -o $(NAME) $(INCLUDE)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean :
	rm -rf $(OBJ)

fclean : clean
		rm -rf $(NAME) $(OBJ)

re = fclean all

