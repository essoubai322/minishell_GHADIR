/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 06:21:06 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/04 10:53:21 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*duplicate_list(t_token *head)
{
	t_token	*new_head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	i = -1;
	new_head = NULL;
	current = head;
	g_glo.tail = NULL;
	while (current != NULL)
	{
		if (initialize_token(&new_token, current))
			return (free(new_head), NULL);
		while (current->args[++i])
			new_token->args[i] = strdup(current->args[i]);
		new_token->args[current->arg_size] = NULL;
		new_token->next = NULL;
		if (!new_head)
			new_head = new_token;
		else
			g_glo.tail->next = new_token;
		g_glo.tail = new_token;
		current = current->next;
	}
	return (new_head);
}

void	cmd_to_start(t_token **current, t_token *pipe_section, t_token *prev)
{
	if ((*current) && (*current)->type == PIPE)
	{
		pipe_section = (*current);
		prev = (*current);
		(*current) = (*current)->next;
		while ((*current))
		{
			if ((*current)->type == CMD)
			{
				if (prev && prev->type == PIPE)
					break ;
				g_glo.start.cmd_token = (*current);
				g_glo.start.cmd_prev = prev;
				while (g_glo.start.cmd_prev
					&& g_glo.start.cmd_prev->next != g_glo.start.cmd_token)
					g_glo.start.cmd_prev = g_glo.start.cmd_prev->next;
				g_glo.start.cmd_prev->next = g_glo.start.cmd_token->next;
				g_glo.start.cmd_token->next = pipe_section->next;
				pipe_section->next = g_glo.start.cmd_token;
				(*current) = g_glo.start.cmd_token;
			}
			prev = (*current);
			(*current) = (*current)->next;
		}
	}
}

int	while_in_cmd_to(t_start *start, t_token **head)
{
	if (start->current->type == CMD)
	{
		if (!start->prev || (start->prev && start->prev->type == PIPE))
			return (1);
		start->cmd_token = start->current;
		start->cmd_prev = start->prev;
		while (start->cmd_prev && start->cmd_prev->next != start->cmd_token)
			start->cmd_prev = start->cmd_prev->next;
		start->cmd_prev->next = start->cmd_token->next;
		if (!start->pipe_section)
		{
			start->cmd_token->next = *head;
			*head = start->cmd_token;
		}
		else
		{
			start->cmd_token->next = start->pipe_section->next;
			start->pipe_section->next = start->cmd_token;
		}
		start->current = start->cmd_token;
	}
	start->prev = start->current;
	start->current = start->current->next;
	return (0);
}

void	reorganize_cmd_to_start(t_token **head)
{
	t_start	start;

	start.pipe_section = NULL;
	start.prev = NULL;
	start.current = *head;
	if (!head || !*head || !(*head)->next)
		return ;
	while (start.current && start.current->type != PIPE)
	{
		if (while_in_cmd_to(&start, head))
			break ;
	}
	cmd_to_start(&start.current, start.pipe_section, start.prev);
}

char	**convert_to_array_v2(t_list *envl, char **g_glo_env)
{
	int		i;
	char	**cenv;
	t_list	*tmp;

	i = 0;
	tmp = envl;
	free_arr(g_glo_env);
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	cenv = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cenv)
		return (NULL);
	i = 0;
	while (envl)
	{
		cenv[i++] = ft_strdup((const char *)envl->content);
		envl = envl->next;
	}
	cenv[i] = 0;
	return (cenv);
}
