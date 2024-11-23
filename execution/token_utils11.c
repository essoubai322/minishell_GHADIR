/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils11.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:11:11 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/23 17:11:16 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token_to_list(t_token **new_head, t_token **current2,
		t_token *new_token)
{
	if (*new_head == NULL)
	{
		*new_head = new_token;
		*current2 = *new_head;
	}
	else
	{
		(*current2)->next = new_token;
		*current2 = (*current2)->next;
	}
}

t_token	*handle_pipe_and_redirects(t_token2 *current)
{
	if (current->type == PIPE1)
		return (create_and_init_token("|", PIPE));
	else if (current->type == HEREDOC1)
		return (create_and_init_token("<<", HEREDOC));
	else if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT
		|| current->type == REDIRECT_APPEND)
		return (create_and_init_token(current->value, RED));
	return (NULL);
}

t_token	*handle_word_types(t_token2 *current, t_token *new_head)
{
	if ((current->type == WORD || current->type == QUOTE
			|| current->type == DQUOTE) && check_last_token(new_head) == RED)
		return (create_and_init_token(current->value, FILE_N));
	return (NULL);
}

int	handle_cmd_args(t_token *current2, const char *value)
{
	char	**new_args;

	current2->arg_size++;
	new_args = ft_realloc(current2->args, (current2->arg_size + 1)
			* sizeof(char *));
	if (!new_args)
		return (0);
	current2->args = new_args;
	current2->args[current2->arg_size - 1] = ft_strdup(value);
	current2->args[current2->arg_size] = NULL;
	return (1);
}

t_token	*handle_cmd(t_token2 *current, t_token *current2)
{
	if (current2 == NULL || current2->type != CMD)
		return (create_and_init_token(current->value, CMD));
	return (NULL);
}
