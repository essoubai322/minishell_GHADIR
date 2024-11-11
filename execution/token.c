/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:25:46 by amoubine          #+#    #+#             */
/*   Updated: 2024/11/04 10:57:13 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

g_global global;

void inialize_global(void)
{
    global.i = 0;
    global.current_token = NULL;
    global.current_token_length = 0;
    global.in_quote = 0;
    global.in_dquote = 0;
    global.error_message = NULL;
    global.apa = NULL;
    global.str_cmd = NULL;
    global.k = 0;
    global.stripped_value = NULL;
    global.env_value = NULL;
    global.o = 0;
}

char *ft_strcat(char *dest, char *src)
{
	int i = 0;
	int j = 0;
	char *result = calloc(ft_strlen(dest) + ft_strlen(src) + 1, sizeof(char));
	while (dest && dest[i])
	{
		result[i] = dest[i];
		i++;
	}
	while (src && src[j])
	{
		result[i] = src[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}

void ft_free2(char ***dest)
{
    if (!dest || !*dest)
        return;

    size_t i = 0;
    while ((*dest)[i])
    {
        free((*dest)[i]);
        (*dest)[i] = NULL;
        i++;
    }
    free(*dest);
    *dest = NULL;
}


t_token2 *create_token(enum TokenType type, const char *value)
{
	t_token2 *new_token = malloc(sizeof(t_token));
	if (!new_token)
        return NULL;	
	new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return (new_token);
}

void    add_token_env_value(char *env_value, char **stripped_value, char *value, int *i)
{
    if (value[*i] == '$' || ft_strchr(value + *i, '$'))
    {
        char *temp = *stripped_value;
        *stripped_value = ft_strcat(*stripped_value, env_value);
        free(temp);
    }
    else
    {
        if (ft_strchr(env_value, ' '))
        {
            char *env_value1 = ft_strcat(env_value, value + *i);
            char *temp = *stripped_value;
            *stripped_value = ft_strcat(*stripped_value, env_value1);
            free(temp);
            free(env_value1);
        }
        else
        {
            char *temp = *stripped_value;
            *stripped_value = ft_strcat(*stripped_value, env_value);
            free(temp);
        }
    }
}

void add_token_innit_head(t_token2 **head, char *stripped_value, enum TokenType type)
{
    t_token2 *new_token;

    new_token = create_token(type, stripped_value);
    if (*head == NULL)
        *head = new_token;
    else
    {
        t_token2 *current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = new_token;
    }
}

int add_token_check(t_token2 **head, char *stripped_value,enum TokenType type)
{
    char    **arg_space;

    arg_space = ft_split(stripped_value, ' ');
    int s = 0;
    while (arg_space && arg_space[s])
        s++;
    if (s > 1)
    {
        s = 0;
        while (arg_space[s])
        {
            add_token_innit_head(head, arg_space[s], type);
            s++;
        }
        ft_free2(&arg_space);
        free(stripped_value);
        return (1);
    }
    ft_free2(&arg_space);
    return (0);
}
char    *ft_get_value(int *i, char *value)
{
    char *env_value = NULL;
    char *var_name = calloc(ft_strlen(value) + 1, sizeof(char));
    (*i)++;
    int k = 0;
    while (value[*i] && (isalnum(value[*i]) || value[*i] == '_'))
        var_name[k++] = value[(*i)++];
    var_name[k] = '\0';
    if (var_name)
        env_value = ft_get_env(var_name, global.env);
    free(var_name);
    return (env_value);
}
void    add_token_else(char *value, int *i, char **stripped_value, int *k)
{
    char *var_name = calloc(ft_strlen(value), sizeof(char));
    while (value[*i] && value[*i] != '$')
    {
        var_name[*k] = value[(*i)++];
        (*k)++;
    }
    *k = 0;

    if (var_name)
    {
        char *temp = *stripped_value;
        *stripped_value = ft_strcat(*stripped_value, var_name);
        free(temp);
    }
    free(var_name);
}

char    *get_stripped_value(enum TokenType type, char *value, char *stripped_value)
{
    global.o = 0;
    global.k = 0;
    stripped_value = calloc(1200500, sizeof(char));
    
    if (type == QUOTE || type == DQUOTE)
    {
        int len = ft_strlen(value);
        if (len > 2)
        {
            strncpy(stripped_value, value + 1, len - 2);
            stripped_value[len - 2] = '\0';
        }
        else
            strcpy(stripped_value, "");
    }
    else if (type == WORD)
        return (stripped_value);
    else
        strcpy(stripped_value, value);
    return (stripped_value);
}

void    add_token_v0(char *value, int *i, char **stripped_value, int *k)
{
    if (value[0] != '$')
    {
        char *name = calloc(ft_strlen(value), sizeof(char));
        while (value[*i] && value[*i] != '$')
            name[(*k)++] = value[(*i)++];
        name[*k] = '\0';
        *k = 0;
        char *temp = *stripped_value;
        *stripped_value = ft_strcat(*stripped_value, name);
        free(temp);
        free(name);
    }
}

void add_token_v1(t_token2 **head, enum TokenType type)
{
    add_token_innit_head(head, global.stripped_value, type);
    free(global.stripped_value);
}


void add_token(t_token2 **head, enum TokenType type, char *value)
{   
    global.stripped_value = get_stripped_value(type, value, global.stripped_value);
    if (type == WORD)
    {
        if (value && strchr(value, '$') != 0)
        {
            add_token_v0(value, &global.o, &global.stripped_value, &global.k);
            while (value[global.o] != '\0')
            {
                if (value[global.o] == '$')
                {
                    global.env_value = ft_get_value(&global.o, value);
                    add_token_env_value(global.env_value, &global.stripped_value, value, &global.o);
                    if (value[global.o] != '$' && ft_strchr(value + global.o, '$') == 0)
                        if (add_token_check(head, global.stripped_value, type))
                            return ;
                }
                else
                    add_token_else(value, &global.o, &global.stripped_value, &global.k);
            }
        }
        else
            strcpy(global.stripped_value, value);
    }
    add_token_v1(head, type);
}

void    add_token_v2(t_token2 **head, enum TokenType type, const char *value)
{
    t_token2 *new_token = create_token(type, value);

    if (*head == NULL) 
        *head = new_token;
    else 
    {
        t_token2 *current = *head;
        while (current->next != NULL) 
            current = current->next;
        
        current->next = new_token;
    }
}

int counter_closes(char *input)
{
    int i = 0;
    int count = 0;
    while(input && input[i])
    {
        if (input[i] == '`')
            count++;
        i++;
    }
    return (count);
}

char *check_case_v1(char **input, int *i, int *k, char *result)
{
    result = calloc(strlen(*input) + 1, sizeof(char));

    while (isspace((*input)[*i]))
        (*i)++;
    while ((*input)[*i] && !isspace((*input)[*i]))
    {
        if ((*input)[*i] == '\'' || (*input)[*i] == '"')
        {
            char quote = (*input)[*i];
            result[(*k)++] = '`';
            (*i)++;
            while ((*input)[*i] && (*input)[*i] != quote)
                result[(*k)++] = (*input)[(*i)++];
            if ((*input)[*i])
                result[(*k)++] = '`';
            else
                return (free(result),NULL);
            (*i)++;
        }
        else
            while((*input)[*i] && !isspace((*input)[*i]) && (*input)[*i] != '\'' && (*input)[*i] != '"')
                result[(*k)++] = (*input)[(*i)++];
    }
    return (result);
}

int check_case(char *input, int i)
{
    char *result = NULL;
    int k = 0;
    
    result = check_case_v1(&input, &i, &k, result);
    if (ft_strchr(result,'`'))
    {
        if (counter_closes(result) == 2)
        {
            if (result[0] != '`' || result[k - 1] != '`')
                return (free(result),1);
        }
        else if (counter_closes(result) && counter_closes(result) % 2 == 0)
            return (free(result),1);
    }
    free(result);
    return(0);
}

int expand_variable(const char *input, int *i, char *result, int *k)
{
    int c = 0;
    (*i)++;
    char *var_name = calloc(strlen(input) + 1, sizeof(char));
    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
        var_name[c++] = input[(*i)++];
    var_name[c] = '\0';
    char *env_value = ft_get_env(var_name, global.env);
    if (env_value)
    {
        char **expanded_tokens = ft_split(env_value, ' ');
        int s = 0;
        while (expanded_tokens[s])
        {
            strcat(result, expanded_tokens[s]);
            if (expanded_tokens[s + 1])
                strcat(result, " ");
            s++;
        }
        ft_free2(&expanded_tokens);
    }
    free(var_name);
    *k = strlen(result);
    return c;
}

void parse_quoted(const char *input, int *i, char *result, int *k, char quote)
{
    (*i)++;
    while (input[*i] && input[*i] != quote)
    {
        if (quote == '"' && input[*i] == '$')
            expand_variable(input, i, result, k);
        else
            result[(*k)++] = input[(*i)++];
    }
    if (input[*i]) (*i)++;
}

void parse_unquoted(const char *input, int *i, char *result, int *k)
{
    while (input[*i] && !isspace(input[*i]) && input[*i] != '\'' && input[*i] != '"' &&
           input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '$')
            expand_variable(input, i, result, k);
        else
            result[(*k)++] = input[(*i)++];
    }
}

char *string_command(const char *input, int *i)
{
    char *result = calloc(10000000, sizeof(char));
    int k = 0;
    while (isspace(input[*i]))
        (*i)++;
    
    while (input[*i] && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '\'' || input[*i] == '"')
            parse_quoted(input, i, result, &k, input[*i]);
        else
            parse_unquoted(input, i, result, &k);
    }
    result[k] = '\0';
    return result;
}

void skip_whitespace(const char *input, int *i)
{
    while (isspace(input[*i]))
        (*i)++;
}

int parse_single_quote(const char *input, int *i, char *result, int *k)
{
    (*i)++;
    while (input[*i] && input[*i] != '\'')
        result[(*k)++] = input[(*i)++];
    if (input[*i])
        (*i)++;
    return 1;
}

int parse_double_quote(const char *input, int *i, char *result, int *k)
{
    (*i)++;
    while (input[*i] && input[*i] != '"')
        result[(*k)++] = input[(*i)++];
    if (input[*i])
        (*i)++;
    return 1;
}

void parse_unquoted_heredoc1(const char *input, int *i, char *result, int *k)
{
    while (input[*i] && !isspace(input[*i]) && input[*i] != '\'' &&
           input[*i] != '"' && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        result[(*k)++] = input[(*i)++];
    }
}

char *after_heredoc1(char *input, int *i)
{
    char *result = calloc(100000, 1);
    int k = 0, flag = 0;
    int after = *i;

    skip_whitespace(input, i);
    while (input[*i] && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '\'')
            flag = parse_single_quote(input, i, result, &k);
        else if (input[*i] == '"')
            flag = parse_double_quote(input, i, result, &k);
        else
            parse_unquoted_heredoc1(input, i, result, &k);
    }
    
    if (!flag && strlen(result) == 0)
    {
        *i = after;
        free(result);
        return NULL;
    }
    result[k] = '\0';
    return result;
}

void while_loop(char *input, t_token2 **head)
{
    if (check_case(input, global.i) && !global.current_token_length)
    {
        global.str_cmd = string_command(input, &global.i);
        global.apa = strdup(global.str_cmd);
        free(global.str_cmd);
        if (ft_strchr(global.apa, ' '))
        {
            char **arg_space = ft_split(global.apa, ' ');
            int s = 0;
            while (arg_space[s])
            {
                add_token_v2(head, WORD, arg_space[s]);
                s++;
            }
            ft_free2(&arg_space);
            free(global.apa);
            return ;
        }
        add_token_v2(head, WORD, global.apa);
        free(global.apa);
    }
    else if (!global.in_quote && !global.in_dquote)
    {
        if (input[global.i] == '|' || input[global.i] == '<' || input[global.i] == '>' || 
            input[global.i] == '\'' || input[global.i] == '"' || isspace(input[global.i])
            || (global.i > 0 && (input[global.i - 1] == '\'' || input[global.i - 1] == '"') &&  input[global.i]))
        {
            if (global.current_token_length > 0) 
            {
                if (input[global.i - 1] == '"')
                    add_token(head, DQUOTE, global.current_token);
                else if (input[global.i - 1] == '\'')
                    add_token(head, QUOTE, global.current_token);
                else
                    add_token(head, WORD, global.current_token);
                global.current_token_length = 0;
                memset(global.current_token, 0, ft_strlen(input));
            }
            if (input[global.i] == '|')
            {
                add_token(head, PIPE1, "|");
                global.i++;
            }
            else if (input[global.i] == '<')
            {
                if (input[global.i + 1] == '<') 
                {
                    add_token(head, HEREDOC1, "<<");
                    global.i += 2;
                    global.apa = after_heredoc1(input, &global.i);
                    if (global.apa)
                        add_token_v2(head, WORD, global.apa);
                    free(global.apa);
                } 
                else 
                {
                    add_token(head, REDIRECT_IN, "<");
                    global.i++;
                }
            }
            else if (input[global.i] == '>')
            {
                if (input[global.i + 1] == '>') 
                {
                    add_token(head, REDIRECT_APPEND, ">>");
                    global.i += 2;
                } 
                else 
                {
                    add_token(head, REDIRECT_OUT, ">");
                    global.i++;
                }
            }
            else if (input[global.i] == '\'')
            {
                global.in_quote = 1;
                global.current_token[global.current_token_length++] = input[global.i];
                global.i++;
            }
            else if (input[global.i] == '"')
            {
                global.in_dquote = 1;
                global.current_token[global.current_token_length++] = input[global.i];
                global.i++;
            }
            else if (isspace(input[global.i]))
                global.i++;
            else if ((input[global.i - 1] == '\'' || input[global.i - 1] == '"') &&  input[global.i])
            {
                global.current_token[global.current_token_length++] = input[global.i];
                global.i++;
            }
        }
        else
        {
            global.current_token[global.current_token_length++] = input[global.i];
            global.i++;
        }
    }
    else if (global.in_quote == 1 && input[global.i] == '\'') 
    {
        global.in_quote = 0;
        global.current_token[global.current_token_length++] = input[global.i];
        global.i++;
    }
    else if (global.in_dquote == 1 && input[global.i] == '"') 
    {
        global.in_dquote = 0;
        global.current_token[global.current_token_length++] = input[global.i];
        global.i++;
    }
    else
    {
        if (input[global.i] == '$' && global.in_dquote == 1)
        {
            char *var_name = calloc(ft_strlen(input) , sizeof(char *));
            int k = 0;

            global.i++;
            while (isalnum(input[global.i]) || input[global.i] == '_')
                var_name[k++] = input[global.i++];
            var_name[k] = '\0';
            
            char *env_value = ft_get_env(var_name, global.env);
            if (env_value)
            {
                strcat(global.current_token, env_value);
                global.current_token_length += ft_strlen(env_value);
            }
            else if (input[global.i - 1] == '\'')
            {
                global.current_token[global.current_token_length++] = '$';
                strcat(global.current_token, var_name);
                global.current_token_length += ft_strlen(var_name);
            }
            free(var_name);
        }
        else
        {
            global.current_token[global.current_token_length++] = input[global.i];
            global.i++;
        }
    }
}

char *check_syntax_errors(t_token2 *head, char *error_message)
{
    t_token2 *current;
    int pipe1_count;

    pipe1_count = 0;
    current = head;
    if (global.in_quote || global.in_dquote) 
        error_message = strdup("bash: unclosed quote detected");
    while (current != NULL && current->next != NULL) 
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT || 
             current->type == REDIRECT_APPEND || current->type == HEREDOC1) && 
            ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
        {
            error_message = strdup("bash: syntax error near unexpected token");
            break;
        }
        current = current->next;
    }   
    current = head;
    while (current != NULL)
    {
        if (current->type == REDIRECT_APPEND || current->type == REDIRECT_IN || current->type == REDIRECT_OUT || current->type == HEREDOC1)
        {
            if (current->next == NULL || ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
            {
                error_message = strdup("bash: syntax error near unexpected token `newline'");
                break;
            }
        }
        current = current->next;
    }
    current = head;
    while (current != NULL) 
    {
        if (current->type == PIPE1) 
        {
            pipe1_count++;
            if (pipe1_count > 0 && current->next != NULL && current->next->type == PIPE1) 
            {
                error_message = strdup("bash: syntax error near unexpected token `|'");
                break;
            }
            if (current == head || current->next == NULL) 
            {
                error_message = strdup("bash: syntax error near unexpected token `|'");
                break;
            }
        } 
        else 
            pipe1_count = 0;

        current = current->next;
    }
    return error_message;
}

t_lexer tokenize(char *input)
{
    t_token2 *head = NULL;
    inialize_global();

	global.current_token = calloc(ft_strlen(input) + 1,sizeof(global.current_token));
    
    while (input[global.i] != '\0')
        while_loop(input, &head);
    if (global.current_token_length > 0) 
    {
		if (input[global.i - 1] == '"')
            add_token(&head, DQUOTE, global.current_token);
		else if (input[global.i - 1] == '\'')
            add_token(&head, DQUOTE, global.current_token);
		else
			add_token(&head, WORD, global.current_token);
    }
    global.error_message = check_syntax_errors(head, global.error_message);
    add_token(&head, END, "");
    t_lexer result = {head, global.error_message};
	free(global.current_token);
    return (result);
}


void free_tokens(t_token2 *head) 
{
    t_token2 *current = head;
    t_token2 *next;

    while (current != NULL) 
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void print_tokens_v2(t_token *head) 
{
    t_token *current = head;
    while (current != NULL) 
    {
        if (current->type == CMD) 
        {
            printf("CMD : args= ");
            for (int i = 0; i < current->arg_size; i++) 
            {
                printf("[%d] '%s'", i,current->args[i]);
                if (i < current->arg_size - 1) 
                    printf(" ");
            }
            printf("\n");
        } 
        else if (current->type == PIPE) 
        {
            printf("PIPE : args= ");
            for (int i = 0; i < current->arg_size; i++) 
            {
                printf("[%d] '%s'", i,current->args[i]);
                if (i < current->arg_size - 1) 
                    printf(" ");
            }
            printf("\n");
        } 
        else if (current->type == RED) 
        {
            printf("RED : args= ");
            for (int i = 0; i < current->arg_size; i++) 
            {
                printf("[%d] '%s'", i, current->args[i]);
                if (i < current->arg_size - 1) 
                    printf(" ");
            }
            printf("\n");
        } 
        else if (current->type == HEREDOC) 
        {
            printf("HEREDOC : args= ");
            for (int i = 0; i < current->arg_size; i++) 
            {
                printf("[%d] '%s'", i, current->args[i]);
                if (i < current->arg_size - 1) 
                    printf(" ");
            }
            printf("\n");
        } 
        else if (current->type == FILE_N) 
        {
            printf("FILE : args= ");
            for (int i = 0; i < current->arg_size; i++) 
            {
                printf("[%d] '%s'", i, current->args[i]);
                if (i < current->arg_size - 1) 
                    printf(" ");
            }
            printf("\n");
        }
        current = current->next;
    }
    printf("\n");
}


void print_tokens(t_token2 *head) 
{
    t_token2 *current = head;
    while (current != NULL && current->type != END) 
    {
        printf("Token type: %d, value: %s\n", current->type, current->value);
        current = current->next;
    }
    printf("\n");
}


t_type check_last_token(t_token *current)
{
    if (current == NULL)
        return (0);
    while (current->next != NULL)
        current = current->next;
    return (current->type);
}

t_token *convert_data(t_token2 *head)
{
    t_token2 *current = head;
    t_token *new_head = NULL;
    t_token *current2 = NULL;

    while (current != NULL && current->type != END) 
    {
        if (current->type == PIPE1) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = PIPE;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup("|");
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        } 
        else if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT || current->type == REDIRECT_APPEND) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = RED;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup(current->value);
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        } 
        else if (current->type == HEREDOC1) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = HEREDOC;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup("<<");
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        }
        else if ((current->type == WORD || current->type == QUOTE || current->type == DQUOTE) &&  check_last_token(new_head) == RED)
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = FILE_N;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup(current->value);
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        }
        else if (current->type == WORD || current->type == QUOTE || current->type == DQUOTE) 
        {
            if (current2 == NULL) 
            {
                t_token *new_token = malloc(sizeof(t_token));
                new_token->type = CMD;
                new_token->arg_size = 1;
                new_token->args = malloc(sizeof(char *) * 2);
                new_token->args[0] = strdup(current->value);
                new_token->args[1] = NULL;
                new_token->next = NULL;
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                if (current2->type == CMD) 
                {
                    current2->arg_size++;
                    current2->args = realloc(current2->args, current2->arg_size * sizeof(char *));
                    current2->args[current2->arg_size - 1] = strdup(current->value);
                    current2->args[current2->arg_size] = NULL;
                } 
                else 
                {
                    t_token *new_token = malloc(sizeof(t_token));
                    new_token->type = CMD;
                    new_token->arg_size = 1;
                    new_token->args = malloc(sizeof(char *) * 2);
                    new_token->args[0] = strdup(current->value);
                    new_token->args[1] = NULL;
                    new_token->next = NULL;
                    current2->next = new_token;
                    current2 = current2->next;
                }
            }
        }
        else if (current->type == END) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = FILE_N;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup(current->value);
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        }
        current = current->next;
    }
    return new_head;
}

int		g_status;

// create function ft_get_env but **en, global.env);v

char	*ft_get_env(char *name, char **env)
{
    int	i;
    int	j;
    int	len;

    i = 0;
    if (!name || !env || !*env)
        return (NULL);
    while (env[i])
    {
        j = 0;
        while (env[i][j] && env[i][j] != '=')
            j++;
        len = j;
        if (ft_strncmp(name, env[i], len) == 0)
            return (env[i] + len + 1);
        i++;
    }
    return (NULL);
}



int	loop(int argc, char **argv, char **env)
{
    char *input;
    (void)argc;
    (void)argv;

    t_list	*lists[2];

    set_up_env_exp(&lists[0], &lists[1], env);

    while (1) 
    {
        global.env = convert_to_array(lists[0]);
        input = readline("minishell> ");
        if (!input) 
        {
            printf("\nExiting minishell...\n");
            break;
        }
        add_history(input);

        t_lexer result = tokenize(input);
        if (result.error_message)
        {
            printf("%s\n", result.error_message);
            free(result.error_message);
        } 
        else 
        {
            t_token *new_head = convert_data(result.tokens);
            print_tokens_v2(new_head);
            if (!new_head)
                continue;
		    excution(&new_head, &lists[0], &lists[1]);
        }
        
        free_tokens(result.tokens);
        free(input);
    }
    rl_clear_history();
    return (0);
}

int	main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
	return (loop(argc, argv,  env));
}
