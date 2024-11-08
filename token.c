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

#include "minishell.h"

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

int	ft_strlen(char *str)
{
	int i = 0;
	while (str && str[i])
		i++;
	return(i);
}

int ft_strchr(const char *s, int c)
{
    if (!s)
        return (0);
    while (*s)
    {
        if (*s == c)
            return (1);
        s++;
    }
    return (0);
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


t_token *create_token(enum TokenType type, const char *value)
{
	t_token *new_token = malloc(sizeof(t_token));
	if (!new_token)
        return NULL;	
	new_token->type = type;
    new_token->value = strdup(value);
	new_token->out = NULL;
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

void add_token_innit_head(t_token **head, char *stripped_value, enum TokenType type)
{
    t_token *new_token;

    new_token = create_token(type, stripped_value);
    if (*head == NULL)
        *head = new_token;
    else
    {
        t_token *current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = new_token;
    }
}

int add_token_check(t_token **head, char *stripped_value,enum TokenType type)
{
    char    **arg_space;

    arg_space = ft_split(stripped_value, ' ');
    int s = 0;
    while (arg_space[s])
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
        env_value = getenv(var_name);
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

void add_token(t_token **head, enum TokenType type, char *value)
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
    add_token_innit_head(head, global.stripped_value, type);
    free(global.stripped_value);
}

void    add_token_v2(t_token **head, enum TokenType type, const char *value)
{
    t_token *new_token = create_token(type, value);

    if (*head == NULL) 
        *head = new_token;
    else 
    {
        t_token *current = *head;
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

int check_case(char *input, int i)
{
    char *result = calloc(strlen(input) + 1, sizeof(char));
    int k = 0;
    
    while (isspace(input[i]))
        (i)++;
    while (input[i] && !isspace(input[i]))
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i];
            result[k++] = '`';
            i++;
            while (input[i] && input[i] != quote)
                result[k++] = input[(i)++];
            if (input[i])
                result[k++] = '`';
            else
                return (free(result),0);
            i++;
        }
        else
        {
            while(input[i] && !isspace(input[i]) && input[i] != '\'' && input[i] != '"')
                result[k++] = input[(i)++];
        }
    }
    result[k] = '\0';
    if (strchr(result,'`'))
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


char *string_command(const char *input, int *i)
{
    char *result = calloc(10000000, sizeof(char));
    int k = 0;
    int c = 0;
    
    while (isspace(input[*i]))
        (*i)++;
    
    while (input[*i] && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            char quote = input[*i];
            (*i)++;
            
            if (quote == '\'')
            {
                while (input[*i] && input[*i] != quote)
                    result[k++] = input[(*i)++];
                if (input[*i])
                    (*i)++;
            }
            else if (quote == '"')
            {
                while (input[*i] && input[*i] != quote)
                {
                    if (input[*i] && input[*i] == '$')
                    {
                        (*i)++;
                        char *var_name = calloc(strlen(input) + 1 , sizeof(char));
                        while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
                            var_name[c++] = input[(*i)++];
                        var_name[c] = '\0';
                        c = 0;

                        char *env_value = getenv(var_name);  
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
                        k = strlen(result);
                    }
                    else
                        result[k++] = input[(*i)++];
                }
                if (input[*i])
                    (*i)++;
            }
        }
        else
        {
            while(input[*i] && !isspace(input[*i]) && input[*i] != '\'' && input[*i] != '"' && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
            {
                if (input[*i] && input[*i] == '$')
                {
                    (*i)++;
                    char *var_name = calloc(strlen(input) + 1 , sizeof(char));
                    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_'))
                        var_name[c++] = input[(*i)++];
                    var_name[c] = '\0';
                    c = 0;

                    char *env_value = getenv(var_name); 
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
                    k = strlen(result);
                }
                else
                    result[k++] = input[(*i)++];
            }
        }
    }
    result[k] = '\0';
    return (result);
}


char *after_heredoc(char *input, int *i)
{
    char *result = calloc(100000,1);
    int k = 0;
    int flag = 0;
    int after = *i;
    
    while (isspace(input[*i]))
        (*i)++;
    
    while (input[*i] && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            flag = 1;
            char quote = input[*i];
            (*i)++;
            
            if (quote == '\'')
            {
                while (input[*i] && input[*i] != quote)
                    result[k++] = input[(*i)++];
                if (input[*i])
                    (*i)++;
            }
            else if (quote == '"')
            {
                while (input[*i] && input[*i] != quote)
                        result[k++] = input[(*i)++];
                if (input[*i])
                    (*i)++;
            }
        }
        else
        {
            while(input[*i] && !isspace(input[*i]) && input[*i] != '\'' && input[*i] != '"' && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
                    result[k++] = input[(*i)++];
        }
    }
    if (!flag && ft_strlen(result) == 0)
    {
        *i = after;
        free(result);
        return (NULL);
    }
    result[k] = '\0';
    return (result);
}


t_lexer tokenize(char *input)
{
    t_token *head = NULL;
    inialize_global();

	global.current_token = calloc(ft_strlen(input) + 1,sizeof(global.current_token));

    while (input[global.i] != '\0')
    {
        if (check_case(input, global.i) && !global.current_token_length)
        {
            global.str_cmd = string_command(input, &global.i);
            global.apa = strdup(global.str_cmd);
            free(global.str_cmd);
            add_token_v2(&head, WORD, global.apa);
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
                    	add_token(&head, DQUOTE, global.current_token);
					else if (input[global.i - 1] == '\'')
            			add_token(&head, QUOTE, global.current_token);
					else
                    	add_token(&head, WORD, global.current_token);
                    global.current_token_length = 0;
                    memset(global.current_token, 0, ft_strlen(input));
                }
                if (input[global.i] == '|')
                {
                    add_token(&head, PIPE, "|");
                    global.i++;
                }
                else if (input[global.i] == '<')
                {
                    if (input[global.i + 1] == '<') 
                    {
                        add_token(&head, HEREDOC, "<<");
                        global.i += 2;
                        global.apa = after_heredoc(input, &global.i);
                        if (global.apa)
                            add_token_v2(&head, WORD, global.apa);
                        free(global.apa);
                    } 
                    else 
                    {
                        add_token(&head, REDIRECT_IN, "<");
                        global.i++;
                    }
                }
                else if (input[global.i] == '>')
                {
                    if (input[global.i + 1] == '>') 
                    {
                        add_token(&head, REDIRECT_APPEND, ">>");
                        global.i += 2;
                    } 
                    else 
                    {
                        add_token(&head, REDIRECT_OUT, ">");
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
				
				char *env_value = getenv(var_name);
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
    if (global.current_token_length > 0) 
    {
		if (input[global.i - 1] == '"')
            add_token(&head, DQUOTE, global.current_token);
		else if (input[global.i - 1] == '\'')
            add_token(&head, DQUOTE, global.current_token);
		else
			add_token(&head, WORD, global.current_token);
    }

    // Check for unclosed quotes
    if (global.in_quote || global.in_dquote) 
    {
        global.error_message = strdup("bash: unclosed quote detected");
    }

    // Check for mismatched redirections
    t_token *current = head;
    while (current != NULL && current->next != NULL) 
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT || 
             current->type == REDIRECT_APPEND || current->type == HEREDOC) && 
            ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
        {
            global.error_message = strdup("bash: syntax error near unexpected token");
            break;
        }
        current = current->next;
    }
   
    // Check for syntax redirection errors
    current = head;
    
    while (current != NULL)
    {
        if (current->type == REDIRECT_APPEND || current->type == REDIRECT_IN || current->type == REDIRECT_OUT || current->type == HEREDOC)
        {
            if (current->next == NULL || ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
            {
                global.error_message = strdup("bash: syntax error near unexpected token `newline'");
                break;
            }
        }
        current = current->next;
    }

    // Check for pipe errors
    current = head;
    int pipe_count = 0;
    while (current != NULL) 
    {
        if (current->type == PIPE) 
        {
            pipe_count++;
            if (pipe_count > 0 && current->next != NULL && current->next->type == PIPE) 
            {
                global.error_message = strdup("bash: syntax error near unexpected token `|'");
                break;
            }
            if (current == head || current->next == NULL) 
            {
                global.error_message = strdup("bash: syntax error near unexpected token `|'");
                break;
            }
        } 
        else 
            pipe_count = 0;

        current = current->next;
    }
    add_token(&head, END, "");
    t_lexer result = {head, global.error_message};
	free(global.current_token);
    return (result);
}

void free_tokens(t_token *head) 
{
    t_token *current = head;
    t_token *next;

    while (current != NULL) 
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void print_tokens(t_token *head) 
{
    t_token *current = head;
    while (current != NULL && current->type != END) 
    {
        printf("Token type: %d, value: %s\n", current->type, current->value);
        current = current->next;
    }
    printf("\n");
}

int main() {
    char *input;

    while (1) 
    {
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
            print_tokens(result.tokens);
        free_tokens(result.tokens);
        free(input);
    }
    rl_clear_history();
    return (0);
}
