/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:22:04 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/27 11:33:10 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_GLOBAL_H
#define MINISHELL_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef struct global 
{
    int i;
    char *current_token;
    int current_token_length;
    int in_quote;
    int in_dquote;
    char *error_message;
    char *apa;
    char *str_cmd;
    int k;
    char *stripped_value;
    int o;
    char *env_value;
} g_global;

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
    struct Token *next;
} t_token;

typedef struct LexerResult {
    t_token *tokens;
    char *error_message;
} t_lexer;

typedef struct s_redict{
    char *file;
    int redict_type;
    struct s_redict *next;
} t_redict;

typedef struct s_mini {
    char **cmd;
    t_redict redi;
    struct s_mini *next;
} t_mini;


int	ft_strlen(char *str);
t_token *create_token(enum TokenType type, const char *value);
void add_token(t_token **head, enum TokenType type, char *value);
t_lexer tokenize(char *input);
void free_tokens(t_token *head);
void print_tokens(t_token *head);
char	**ft_split(char const *s1, char c);
void    add_token_v2(t_token **head, enum TokenType type, const char *value);


#endif