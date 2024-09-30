/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 19:58:54 by asebaai           #+#    #+#             */
/*   Updated: 2024/09/30 20:01:24 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

void	add_token_v2(t_token **head, enum TokenType type, const char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(type, value);
	if (*head == NULL)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_token;
	}
}

int	counter_closes(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input && input[i])
	{
		if (input[i] == '`')
			count++;
		i++;
	}
	return (count);
}

void	ft_free2(char **dest)
{
	size_t	i;

	i = 0;
	if (!dest || !*dest)
	{
		free(dest);
		dest = NULL;
		return ;
	}
	while (dest[i])
	{
		free(dest[i]);
		dest[i] = NULL;
		i++;
	}
	free(dest);
	dest = NULL;
}

t_token	*create_token(enum TokenType type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	new_token->type = type;
	new_token->value = strdup(value);
	new_token->out = NULL;
	new_token->next = NULL;
	return (new_token);
}
