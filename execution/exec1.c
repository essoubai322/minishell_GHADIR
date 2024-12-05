/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:50:05 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/23 09:13:55 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipe(t_token *list)
{
	while (list)
	{
		if (list->args[0] && list->type == PIPE)
			return (1);
		list = list->next;
	}
	return (0);
}

void	printf_error(char *str, char *cmd, int exit_status)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	if (exit_status)
		g_glo.sts = exit_status;
}

int	check_redir(t_token *head, int f)
{
	while (head)
	{
		if (head->args[0] && head->args[0][0] == '>' && !f)
			return (head->arg_size);
		if (head->args[0] && head->args[0][0] == '<' && f)
			return (1);
		head = head->next;
	}
	return (0);
}

void	execve_error(char *cmd, t_token *head, t_list **lists[2], char **paths)
{
	struct stat	st;

	stat(cmd, &st);
	if (S_ISDIR(st.st_mode))
	{
		printf_error("is a directory", cmd, 126);
		ft_lstclear(lists[0], &del);
		ft_lstclear(lists[1], &del);
		list_clear(&head);
		(free_arr(paths), free(cmd));
		exit(126);
	}
	else if (access(cmd, F_OK) == 0)
	{
		printf_error("command not found", cmd, 127);
		ft_lstclear(lists[0], &del);
		ft_lstclear(lists[1], &del);
		list_clear(&head);
		(free_arr(paths), free(cmd));
		exit(127);
	}
	ft_lstclear(lists[0], &del);
	ft_lstclear(lists[1], &del);
	(list_clear(&head), free_arr(paths), free(cmd));
	exit(EXIT_FAILURE);
}

void	free_run_cmd(char **paths, char **env, char ***arr, int flag)
{
	free_arr(paths);
	if (flag)
		free_arr(env);
	else
		free(env);
	*arr = NULL;
}
