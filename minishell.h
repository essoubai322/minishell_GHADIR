/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:22:04 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/26 10:51:00 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>


enum TokenType {
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
    HEREDOC,
    QUOTE,
    DQUOTE,
    END
};

typedef struct Token {
    enum TokenType type;
    char *value;
	char *out;
    struct Token *next;
} t_token;

typedef struct LexerResult {
    t_token *tokens;
    char *error_message;
} t_lexer;

int	ft_strlen(char *str);
t_token *create_token(enum TokenType type, const char *value);
void add_token(t_token **head, enum TokenType type, char *value);
t_lexer tokenize(char *input);
void free_tokens(t_token *head);
void print_tokens(t_token *head);
char	**ft_split(char const *s1, char c);


#endif