/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:25:46 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/30 20:42:09 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void	add_token(t_token **head, enum TokenType type, char *value)
{
	t_tokenizer	tz;
	t_token		*new_token;

	tz.stripped_value = calloc(10000000, sizeof(char));
	tz.i = 0;
	tz.k = 0;
	tz.s = 0;
	if (type == QUOTE || type == DQUOTE)
		handle_quote(&tz, value);
	else if (type == WORD)
		handle_word(&tz, value, head, type);
	else
		strcpy(tz.stripped_value, value);
	new_token = create_token(type, tz.stripped_value);
	free(tz.stripped_value);
	add_new_token(head, new_token);
}

int	check_case(char *input, int i)
{
	char	*result;
	int		k;
	char	quote;

	result = calloc(strlen(input) + 1, sizeof(char));
	k = 0;
	while (isspace(input[i]))
		(i)++;
	while (input[i] && !isspace(input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			result[k++] = '`';
			i++;
			while (input[i] && input[i] != quote)
				result[k++] = input[(i)++];
			if (input[i])
				result[k++] = '`';
			else
				return (free(result), 0);
			i++;
		}
		else
		{
			while (input[i] && !isspace(input[i]) && input[i] != '\''
				&& input[i] != '"')
				result[k++] = input[(i)++];
		}
	}
	result[k] = '\0';
	if (strchr(result, '`'))
	{
		if (counter_closes(result) == 2)
		{
			if (result[0] != '`' || result[k - 1] != '`')
				return (free(result), 1);
		}
		else if (counter_closes(result) && counter_closes(result) % 2 == 0)
			return (free(result), 1);
	}
	free(result);
	return (0);
}

char	*string_command(const char *input, int *i)
{
	char	*result;
	int		k;
	int		c;
	char	quote;
	char	*var_name;
	char	*env_value;

	result = calloc(10000000, sizeof(char));
	k = 0;
	c = 0;
	while (isspace(input[*i]))
		(*i)++;
	while (input[*i] && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
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
						var_name = calloc(strlen(input) + 1, sizeof(char *));
						while (input[*i] && (isalnum(input[*i])
								|| input[*i] == '_'))
							var_name[c++] = input[(*i)++];
						var_name[c] = '\0';
						c = 0;
						env_value = getenv(var_name);
						if (env_value)
							strcat(result, env_value);
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
			while (input[*i] && !isspace(input[*i]) && input[*i] != '\''
				&& input[*i] != '"' && input[*i] != '|' && input[*i] != '>'
				&& input[*i] != '<')
			{
				if (input[*i] && input[*i] == '$')
				{
					(*i)++;
					var_name = calloc(strlen(input) + 1, sizeof(char *));
					while (input[*i] && (isalnum(input[*i])
							|| input[*i] == '_'))
						var_name[c++] = input[(*i)++];
					var_name[c] = '\0';
					c = 0;
					env_value = getenv(var_name);
					if (env_value)
						strcat(result, env_value);
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

char	*after_heredoc(char *input, int *i)
{
	char	*result;
	int		k;
	int		flag;
	int		after;
	char	quote;

	result = calloc(100000, 1);
	k = 0;
	flag = 0;
	after = *i;
	while (isspace(input[*i]))
		(*i)++;
	while (input[*i] && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			flag = 1;
			quote = input[*i];
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
			while (input[*i] && !isspace(input[*i]) && input[*i] != '\''
				&& input[*i] != '"' && input[*i] != '|' && input[*i] != '>'
				&& input[*i] != '<')
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

t_lexer	tokenize(char *input)
{
	t_token	*head;
	int		i;
	char	*current_token;
	int		current_token_length;
	int		in_quote;
	int		in_dquote;
	char	*error_message;
	char	*apa;
	char	*str_cmd;
	char	*var_name;
	int		k;
	char	*env_value;
	t_token	*current;
	int		pipe_count;
	t_lexer	result;

	head = NULL;
	i = 0;
	current_token_length = 0;
	in_quote = 0;
	in_dquote = 0;
	error_message = NULL;
	apa = NULL;
	str_cmd = NULL;
	current_token = calloc(ft_strlen(input) + 1, sizeof(current_token));
	while (input[i] != '\0')
	{
		if (check_case(input, i) && !current_token_length)
		{
			str_cmd = string_command(input, &i);
			apa = strdup(str_cmd);
			free(str_cmd);
			add_token_v2(&head, WORD, apa);
			free(apa);
		}
		else if (!in_quote && !in_dquote)
		{
			if (input[i] == '|' || input[i] == '<' || input[i] == '>'
				|| input[i] == '\'' || input[i] == '"' || isspace(input[i])
				|| (i > 0 && (input[i - 1] == '\'' || input[i - 1] == '"')
				&& input[i]))
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
						apa = after_heredoc(input, &i);
						if (apa)
							add_token_v2(&head, WORD, apa);
						free(apa);
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
				else if ((input[i - 1] == '\'' || input[i - 1] == '"')
						&& input[i])
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
				var_name = calloc(ft_strlen(input), sizeof(char *));
				k = 0;
				i++;
				while (isalnum(input[i]) || input[i] == '_')
					var_name[k++] = input[i++];
				var_name[k] = '\0';
				env_value = getenv(var_name);
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
	current = head;
	while (current != NULL && current->next != NULL)
	{
		if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT
				|| current->type == REDIRECT_APPEND || current->type == HEREDOC)
			&& ((current->next->type != WORD) && (current->next->type != DQUOTE)
				&& (current->next->type = QUOTE)))
		{
			error_message = strdup("bash: syntax error near unexpected token");
			break ;
		}
		current = current->next;
	}
	// Check for syntax redirection errors
	current = head;
	while (current != NULL)
	{
		if (current->type == REDIRECT_APPEND || current->type == REDIRECT_IN
			|| current->type == REDIRECT_OUT || current->type == HEREDOC)
		{
			if (current->next == NULL || ((current->next->type != WORD)
					&& (current->next->type != DQUOTE)
					&& (current->next->type = QUOTE)))
			{
				error_message = strdup("bash: syntax error near unexpected token `newline'");
				break ;
			}
		}
		current = current->next;
	}
	// Check for pipe errors
	current = head;
	pipe_count = 0;
	while (current != NULL)
	{
		if (current->type == PIPE)
		{
			pipe_count++;
			if (pipe_count > 0 && current->next != NULL
				&& current->next->type == PIPE)
			{
				error_message = strdup("bash: syntax error near unexpected token `|'");
				break ;
			}
			if (current == head || current->next == NULL)
			{
				error_message = strdup("bash: syntax error near unexpected token `|'");
				break ;
			}
		}
		else
			pipe_count = 0;
		current = current->next;
	}
	add_token(&head, END, "");
	result.tokens = head;
	result.error_message = error_message;
	free(current_token);
	return (result);
}

void	print_tokens(t_token *head)
{
	t_token	*current;

	current = head;
	while (current != NULL && current->type != END)
	{
		printf("Token type: %d, value: %s\n", current->type, current->value);
		current = current->next;
	}
	printf("\n");
}

int	main(void)
{
	char *input;

	while (1)
	{
		input = readline("minishell> ");

		if (!input)
		{
			printf("\nExiting minishell...\n");
			break ;
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