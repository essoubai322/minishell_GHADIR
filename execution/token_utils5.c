/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 06:11:35 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/23 17:23:27 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*after_heredoc1(char *input, int *i)
{
	char	*result;
	int		k = 0, flag;
	int		after;

	result = calloc(100000, 1);
	k = 0, flag = 0;
	after = *i;
	skip_whitespace(input, i);
	while (input[*i] && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
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
		return (NULL);
	}
	result[k] = '\0';
	return (result);
}

t_lexer	tokenize(char *input)
{
	t_token2	*head;
	t_lexer		result;

	head = NULL;
	inialize_global();
	global.current_token = calloc(ft_strlen(input) + 1, \
		sizeof(global.current_token));
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
	result.tokens = head;
	result.error_message = global.error_message;
	free(global.current_token);
	return (result);
}

void	free_tokens(t_token2 *head)
{
	t_token2	*current;
	t_token2	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

t_type	check_last_token(t_token *current)
{
	if (current == NULL)
		return (0);
	while (current->next != NULL)
		current = current->next;
	return (current->type);
}

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
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}
