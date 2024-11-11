/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:53:02 by rbenmakh          #+#    #+#             */
/*   Updated: 2024/09/16 09:47:42 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	calc_pipes(t_token *list)
{
	int	p;

	p = 0;
	while (list)
	{
		if (list->args[0] && list->args[0][0] == '|')
			p++;
		list = list->next;
	}
	return (p);
}

char	**convert_to_array(t_list *envl)
{
	int		i;
	char	**cenv;
	t_list	*tmp;

	i = 0;
	tmp = envl;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	cenv = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cenv)
		return (0);
	i = 0;
	while (envl)
	{
		cenv[i++] = (char *)envl->content;
		envl = envl->next;
	}
	cenv[i] = 0;
	return (cenv);
}

void	excution(t_token **head, t_list **envl, t_list **exp_list)
{
	t_token	*list[2];
	list[0] = *head;
	list[1] = *head;
	if (check_pipe(*head))
		exec_pipes(list, envl, exp_list, split_paths(get_path(*envl)));
	else if (check_redir(*head, 0) || check_redir(*head, 1))
		redirection(*head, envl, exp_list);
	else
		run_cmd(*head, envl, exp_list, split_paths(get_path(*envl)));
	list_clear(head);
	*head = NULL;
}

void	clear_child(t_token *head, t_list **envl, t_list **exp_list,
		char **paths)
{
	free_arr(paths);
	ft_lstclear(envl, &del);
	ft_lstclear(exp_list, &del);
	list_clear(&head);
}

void	free_str(char **str1, char **str2)
{
	if (*str1 != *str2)
	{
		free(*str1);
	}
}
