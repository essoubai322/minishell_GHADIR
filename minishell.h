/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:22:04 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/30 20:35:09 by asebaai          ###   ########.fr       */
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


typedef struct s_tokenizer
{
    char *stripped_value;
    char **arg_space;
    int i;
    int k;
    int s;
} t_tokenizer;

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
void    add_token_v2(t_token **head, enum TokenType type, const char *value);
void free_tokens(t_token *head);
int counter_closes(char *input);
void ft_free2(char **dest);
void handle_quote(t_tokenizer *tz, char *value);
void handle_env_var(t_tokenizer *tz, char *value);
void add_new_token(t_token **head, t_token *new_token);
int count_arg_space(t_tokenizer *tz);
int process_arg_space(t_tokenizer *tz, t_token **head, enum TokenType type);
void handle_dollar_sign(t_tokenizer *tz, char *value, t_token **head, enum TokenType type);
void handle_non_dollar_sign(t_tokenizer *tz, char *value);
void handle_word_with_dollar(t_tokenizer *tz, char *value, t_token **head, enum TokenType type);
void handle_word_with_dollar_middle(t_tokenizer *tz, char *value, t_token **head, enum TokenType type);
void handle_word(t_tokenizer *tz, char *value, t_token **head, enum TokenType type);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*malloc_1(size_t size, char const *index);

#endif