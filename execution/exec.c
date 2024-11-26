/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:38:20 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/23 11:30:24 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*search_path(char *cmd, char **paths, int *num)
{
	int		i;
	char	*tmp;
	char	*tmp1;

	i = 0;
	if (cmd[0] == '\0')
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin("/", cmd);
		tmp1 = tmp;
		tmp = ft_strjoin(paths[i], tmp);
		free(tmp1);
		*num = access(tmp, F_OK | X_OK);
		if (!*num)
			return (tmp);
		free(tmp);
		i++;
	}
	if (!access(cmd, F_OK | X_OK))
		return (cmd);
	return (NULL);
}

char	*check_cmd(char *cmd, char **paths)
{
	int		num;
	char	*tmp;

	num = -1;
	if ((cmd[0] == '.' || cmd[0] == '/') && access(cmd, F_OK | X_OK) != 0)
	{
		perror("minishell");
		return (NULL);
	}
	else if ((cmd[0] == '.' || cmd[0] == '/') && !access(cmd, F_OK | X_OK))
		return (cmd);
	else if (!paths)
		return (write(2, "minishell: : No such file or directory\n", 40), NULL);
	tmp = search_path(cmd, paths, &num);
	if (tmp)
		return (tmp);
	if (num == -1 && check_builtin(cmd))
	{
		printf_error("command not found", cmd, 127);
		cmd = NULL;
	}
	return (cmd);
}

int	builtin(t_token *head, t_list **envl, t_list **exp_list)
{
	if (!head->args[0])
		return (1);
	if (!ft_strncmp(head->args[0], "cd", 3))
		return (cd(head->args, envl, exp_list, NULL));
	else if (!ft_strncmp(head->args[0], "echo", 5))
		return (echo(head->args), 0);
	else if (!ft_strncmp(head->args[0], "export", 7))
	{
		if (head->arg_size == 1)
			export(exp_list, envl, NULL, NULL);
		return (init_export(head, envl, exp_list, 0), g_glo.sts);
	}
	else if (!ft_strncmp(head->args[0], "unset", 6))
	{
		unset(envl, head->args[1], 0);
		unset(exp_list, head->args[1], 1);
		return (0);
	}
	else if (!ft_strncmp(head->args[0], "env", 4) && !head->args[1])
		return (print_env(*envl), 0);
	else if (!ft_strncmp(head->args[0], "env", 4) && head->args[1])
		return (write(2, "env: too many arguments\n", 25), 0);
	else if (!ft_strncmp(head->args[0], "pwd", 4))
		return (pwd(1, *envl), 0);
	return (1);
}

int	essential_cmd(t_token *head, char **paths, t_list **lists[2], char **cmd)
{
	if (head->args[0] && !ft_strncmp(head->args[0], "exit", 5))
	{
		ft_exit(head, lists, paths, 0);
		g_glo.sts = 1;
		return (0);
	}
	if (!head->args[0] || (head->args[0] && head->args[0][0] == '>'))
	{
		free_arr(paths);
		g_glo.sts = 0;
		return (0);
	}
	else if ((head->args[0] && head->args[0][0] == '<'))
		return (free_arr(paths), 0);
	g_glo.sts = builtin(head, lists[0], lists[1]);
	if (!g_glo.sts)
		return (free_arr(paths), 0);
	*cmd = check_cmd(head->args[0], paths);
	if (!*cmd)
		return (free_arr(paths), 0);
	return (1);
}

void	run_cmd(t_token *head, t_list **envl, t_list **exp_list, char **paths)
{
	char	*cmd;
	char	**env;
	int		num[2];
	t_list	**lists[2];

	setup_lists(lists, envl, exp_list);
	if (!essential_cmd(head, paths, lists, &cmd))
		return ;
	num[0] = 0;
	env = convert_to_array(*envl);
	signal_setup(0);
	num[1] = fork();
	if (!num[1])
	{
		signal_setup(3);
		if (execve(cmd, head->args, env) != 0)
		{
			free(env);
			execve_error(cmd, head, lists, paths);
		}
	}
	else
		wait(&num[0]);
	exit_status(num[0]);
	cleanup_run_cmd(&env, &cmd, paths, head);
}
