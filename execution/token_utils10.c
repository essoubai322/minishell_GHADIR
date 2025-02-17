/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils10.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:06:42 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/04 09:34:31 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_syntax_error_v2(t_token2 *current, char **error)
{
	while (current != NULL)
	{
		if (current->type == REDIRECT_APPEND || current->type == REDIRECT_IN
			|| current->type == REDIRECT_OUT || current->type == HEREDOC1)
		{
			if (current->next == NULL || ((current->next->type != WORD)
					&& (current->next->type != DQUOTE)
					&& (current->next->type != QUOTE)))
			{
				*error = ft_strdup
					("bash: syntax error near unexpected token `newline'");
				break ;
			}
		}
		current = current->next;
	}
}

void	check_syntax_error_v3(t_token2 *head, t_token2 *current, char **error,
		int pipe1_count)
{
	while (current != NULL)
	{
		if (current->type == PIPE1)
		{
			pipe1_count++;
			if (pipe1_count > 0 && current->next != NULL
				&& current->next->type == PIPE1)
			{
				*error = ft_strdup
					("bash: syntax error near unexpected token `|'");
				break ;
			}
			if (current == head || current->next == NULL)
			{
				*error = ft_strdup
					("bash: syntax error near unexpected token `|'");
				break ;
			}
		}
		else
			pipe1_count = 0;
		current = current->next;
	}
}

char	*check_syntax_errors(t_token2 *head, char *error_message)
{
	t_token2	*current;
	int			pipe1_count;

	pipe1_count = 0;
	current = head;
	check_syntax_error_v1(current, &error_message);
	current = head;
	check_syntax_error_v2(current, &error_message);
	current = head;
	check_syntax_error_v3(head, current, &error_message, pipe1_count);
	return (error_message);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;
	char	**dest;
	char	**src;
	size_t	i;

	if (!ptr)
		return (malloc(size));
	if (size == 0 && ptr)
		return (free(ptr), NULL);
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	dest = (char **)new_ptr;
	src = (char **)ptr;
	i = 0;
	while (src[i] && i < (size / sizeof(char *) - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = NULL;
	free(ptr);
	return (new_ptr);
}

t_token	*convert_data(t_token2 *head, t_token *current2, t_token *new_token)
{
	g_glo.cur = head;
	g_glo.new_head = NULL;
	current2 = NULL;
	while (g_glo.cur != NULL && g_glo.cur->type != END)
	{
		new_token = handle_pipe_and_redirects(g_glo.cur);
		if (!new_token)
			new_token = handle_word_types(g_glo.cur, g_glo.new_head);
		if (!new_token && (g_glo.cur->type == WORD || g_glo.cur->type == QUOTE
				|| g_glo.cur->type == DQUOTE))
		{
			new_token = handle_cmd(g_glo.cur, current2);
			if (!new_token && current2)
			{
				if (!handle_cmd_args(current2, g_glo.cur->value))
					return (NULL);
				g_glo.cur = g_glo.cur->next;
				continue ;
			}
		}
		if (new_token)
			add_token_to_list(&g_glo.new_head, &current2, new_token);
		g_glo.cur = g_glo.cur->next;
	}
	return (g_glo.new_head);
}
