# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amoubine <amoubine@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/26 10:12:54 by amoubine          #+#    #+#              #
#    Updated: 2024/11/04 09:29:47 by amoubine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wextra -Werror -Wall -g3 #-fsanitize=address 

INCLUDE = -lreadline

SOURCES = token.c split.c

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

