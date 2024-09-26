/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubine <amoubine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:25:46 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/26 11:17:34 by amoubine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int i = 0;
	while (str && str[i])
		i++;
	return(i);
}
void	ft_free2(char **dest)
{
	size_t	i;

	i = 0;
	while (dest[i])
	{
		free(dest[i]);
		dest[i] = NULL;
		i++;
	}
	free(dest);
	dest = NULL;
}

t_token *create_token(enum TokenType type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    new_token->type = type;
    new_token->value = strdup(value);
	new_token->out = NULL;
    new_token->next = NULL;
    return (new_token);
}

void add_token(t_token **head, enum TokenType type, char *value)
{
	char *stripped_value = calloc(10000000 , sizeof(char *));
	char **arg_space;
    if (type == QUOTE || type == DQUOTE)
	{
        int len = ft_strlen(value);
        if (len > 2)
		{
            strncpy(stripped_value, value + 1, len - 2);
            stripped_value[len - 2] = '\0';
        } 
		else 
            strcpy(stripped_value,"");
    }
	else if (type == WORD) 
	{
		int i = 0;
		int k = 0;
		if (value && value[0] == '$')
		{
			while (value[i] != '\0')
			{
				if (value[i] == '$')
				{
					char *var_name = calloc(ft_strlen(value) , sizeof(char *));
					i++;
					while (value[i] && (isalnum(value[i]) || value[i] == '_'))
						var_name[k++] = value[i++];
					var_name[k] = '\0';
					k = 0;
					char *env_value = getenv(var_name);
					if (env_value)
						strcat(stripped_value, env_value);
					free(var_name);
					arg_space = ft_split(stripped_value ,' ');
					int s = 0;
					while (arg_space[s])
						s++;
					if (s > 1)
					{
						free(stripped_value);
						t_token *new_token = NULL;
						s = 0;
						while (arg_space[s])
						{
							new_token = create_token(type, arg_space[s]);
							if (*head == NULL) 
								*head = new_token;
							else 
							{
								t_token *current = *head;
								while (current->next != NULL) 
									current = current->next;
								current->next = new_token;
							}
							s++;
						}
						ft_free2(arg_space);
						return ;
					}
					
				}
				else
				{
					char *var_name = calloc(ft_strlen(value) , sizeof(char *));
					while (value[i] && value[i] != '$')
						var_name[k++] = value[i++];
					var_name[k] = '\0';
					k = 0;
					if (var_name)
						strcat(stripped_value, var_name);
					free(var_name);
				}
			}
		}
		else if (strchr(value, '$') != 0)
		{
			char *name = calloc(ft_strlen(value) , sizeof(char *));
			while (value[i] && value[i] != '$')
				name[k++] = value[i++];
			name[k] = '\0';
			k = 0;
			strcat(stripped_value, name);
			free(name);
			while (value[i] != '\0')
			{
				if (value[i] == '$')
				{
					i++;
					char *var_name = calloc(ft_strlen(value) , sizeof(char *));
					while (value[i] && isalnum(value[i]) || value[i] == '_')
						var_name[k++] = value[i++];
					var_name[k] = '\0';
					k = 0;
					char *env_value = getenv(var_name);
					if (env_value)
						strcat(stripped_value,env_value);
					free(var_name);
					arg_space = ft_split(stripped_value ,' ');
					int s = 0;
					while (arg_space[s])
						s++;
					if (s > 1)
					{
						free(stripped_value);
						t_token *new_token = NULL;
						s = 0;
						while (arg_space[s])
						{
							new_token = create_token(type, arg_space[s]);
							if (*head == NULL) 
								*head = new_token;
							else 
							{
								t_token *current = *head;
								while (current->next != NULL) 
									current = current->next;
								current->next = new_token;
							}
							s++;
						}
						ft_free2(arg_space);
						return ;
					}
				}
				else
				{
					char *var_name = calloc(ft_strlen(value) , sizeof(char *));
					while (value[i] && value[i] != '$')
						var_name[k++] = value[i++];
					var_name[k] = '\0';
					k = 0;
					if (var_name)
						strcat(stripped_value, var_name);
					free(var_name);
				}
			}
			
		}
		else 
        	strcpy(stripped_value,value);
    }
	else 
        strcpy(stripped_value,value);

    t_token *new_token = create_token(type, stripped_value);
	free(stripped_value);

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

t_lexer tokenize(char *input)
{
    t_token *head = NULL;
    int i = 0;
    char *current_token;
    int current_token_length = 0;
    int in_quote = 0;
    int in_dquote = 0;
    char *error_message = NULL;

	current_token = calloc(ft_strlen(input) + 1,sizeof(current_token));

    while (input[i] != '\0')
    {
        if (!in_quote && !in_dquote)
        {
            if (input[i] == '|' || input[i] == '<' || input[i] == '>' || 
                input[i] == '\'' || input[i] == '"' || isspace(input[i])
				|| i > 0 && ((input[i - 1] == '\'' || input[i - 1] == '"') &&  input[i]))
            {
                if (current_token_length > 0) 
                {
					if (input[i - 1] == '"')
                    	add_token(&head, DQUOTE, current_token);
					else if (input[i - 1] == '\'')
            			add_token(&head, QUOTE, current_token);
					else
                    	add_token(&head, WORD, current_token);
                    current_token_length = 0;
                    memset(current_token, 0, ft_strlen(input));
                }
                if (input[i] == '|')
                {
                    add_token(&head, PIPE, "|");
                    i++;
                }
                else if (input[i] == '<')
                {
                    if (input[i + 1] == '<') 
                    {
                        add_token(&head, HEREDOC, "<<");
                        i += 2;
                    } 
                    else 
                    {
                        add_token(&head, REDIRECT_IN, "<");
                        i++;
                    }
                }
                else if (input[i] == '>')
                {
                    if (input[i + 1] == '>') 
                    {
                        add_token(&head, REDIRECT_APPEND, ">>");
                        i += 2;
                    } 
                    else 
                    {
                        add_token(&head, REDIRECT_OUT, ">");
                        i++;
                    }
                }
                else if (input[i] == '\'')
                {
                    in_quote = 1;
                    current_token[current_token_length++] = input[i];
                    i++;
                }
                else if (input[i] == '"')
                {
                    in_dquote = 1;
                    current_token[current_token_length++] = input[i];
                    i++;
                }
                else if (isspace(input[i]))
                    i++;
				else if ((input[i - 1] == '\'' || input[i - 1] == '"') &&  input[i])
				{
					current_token[current_token_length++] = input[i];
					i++;
				}
            }
            else
            {
                current_token[current_token_length++] = input[i];
                i++;
            }
        }
        else if (in_quote == 1 && input[i] == '\'') 
        {
            in_quote = 0;
            current_token[current_token_length++] = input[i];
            i++;
        }
        else if (in_dquote == 1 && input[i] == '"') 
        {
            in_dquote = 0;
            current_token[current_token_length++] = input[i];
            i++;
        }
        else
        {
			if (input[i] == '$' && in_dquote == 1)
			{
				char *var_name = calloc(ft_strlen(input) , sizeof(char *));
				int k = 0;

				i++;
				while (isalnum(input[i]) || input[i] == '_')
					var_name[k++] = input[i++];
				var_name[k] = '\0';
				
				char *env_value = getenv(var_name);
				if (env_value)
				{
					strcat(current_token, env_value);
					current_token_length += ft_strlen(env_value);
				}
				else if (input[i - 1] == '\'')
				{
					current_token[current_token_length++] = '$';
					strcat(current_token, var_name);
					current_token_length += ft_strlen(var_name);
				}
				free(var_name);
			}
			else
			{
            	current_token[current_token_length++] = input[i];
            	i++;
			}
        }
    }
    if (current_token_length > 0) 
    {
		if (input[i - 1] == '"')
            add_token(&head, DQUOTE, current_token);
		else if (input[i - 1] == '\'')
            add_token(&head, DQUOTE, current_token);
		else
			add_token(&head, WORD, current_token);
    }

    // Check for unclosed quotes
    if (in_quote || in_dquote) 
    {
        error_message = strdup("bash: unclosed quote detected");
    }

    // Check for mismatched redirections
    t_token *current = head;
    while (current != NULL && current->next != NULL) 
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT || 
             current->type == REDIRECT_APPEND || current->type == HEREDOC) && 
            ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
        {
            error_message = strdup("bash: syntax error near unexpected token");
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
                error_message = strdup("bash: syntax error near unexpected token `newline'");
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
            pipe_count = 0;

        current = current->next;
    }
    add_token(&head, END, "");
    t_lexer result = {head, error_message};
	free(current_token);
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
// char *env_var(char *input)
// {
// 	int i = 0;
// 	int k = 0;
// 	char *var = calloc(ft_strlen(input),sizeof(char *));
// 	while (input[i] && input[i] == '$' && input[i + 1] == '$')
// 		var[k++] = input[i++];
// 	var[k] = '\0';
// 	printf("%s" , var);
// 	return(var);
// }

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