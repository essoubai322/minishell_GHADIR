/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:38:08 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/07 08:17:58 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_pipe	*init_pipes(int p)
{
	t_pipe	*fdt;
	int		i;

	i = 0;
	fdt = malloc(sizeof(t_pipe) * p);
	if (!fdt)
		return (0);
	while (i < p)
	{
		if (pipe(fdt[i].fd) == -1)
		{
			(void)fdt;
		}
		i++;
	}
	return (fdt);
}

void	pipe_redirection(t_token *head)
{
	int		r;
	t_token	*tmp;

	r = check_redir(head, 0);
	if (r || check_redir(head, 1))
	{
		tmp = head;
		while (tmp)
		{
			if (tmp->args[0] && tmp->args[0][0] == '|')
				break ;
			else if (tmp->args[0] && tmp->args[0][0] == '>')
				redir_output(tmp->next->args[0], r);
			else if (tmp->args[0] && tmp->args[0][0] == '<')
			{
				if (check_input_redirection(tmp->next->args[0]))
					break ;
				redir_input(tmp->next->args[0]);
			}
			tmp = tmp->next;
		}
	}
}

void	check_invalid_cmd(t_token *head, t_list **envl,
		t_list **exp_list, char **paths)
{
	if (!head->args[0] || (head->args[0] && head->args[0][0] == '>')
		|| (head->args[0] && head->args[0][0] == '<'))
	{
		clear_child(head, envl, exp_list, paths);
		exit(1);
	}
}

void	exe_v2(char *cmd)
{
	struct stat	st;

	stat(cmd, &st);
	if (S_ISDIR(st.st_mode))
	{
		printf_error("is a directory", cmd, 126);
		free(cmd);
		exit(126);
	}
	else if (access(cmd, F_OK) == 0)
	{
		printf_error("command not found", cmd, 127);
		free(cmd);
		exit(127);
	}
	free(cmd);
	exit(EXIT_FAILURE);
}

void	run(t_token *list[2], t_list **envl, t_list **et, char **paths)
{
	char	*cmd;
	char	**env;

	signal_setup(1);
	pipe_redirection(list[1]);
	check_invalid_redirection(list[1], envl, et, paths);
	check_invalid_cmd(list[1], envl, et, paths);
	if (ft_strnstr(list[1]->args[0], "exit", ft_strlen("exit")))
		ft_exit_pipe(list[0], envl, et, paths);
	else if (!builtin(list[1], envl, et))
	{
		clear_child(list[0], envl, et, paths);
		exit(0);
	}
	cmd = check_cmd(list[1]->args[0], paths);
	if (!cmd)
	{
		clear_child(list[0], envl, et, paths);
		exit(127);
	}
	env = convert_to_array(*envl);
	if (execve(cmd, list[1]->args, env) == -1)
		(clear_child(list[0], envl, et, paths), free(env), exe_v2(cmd));
}
