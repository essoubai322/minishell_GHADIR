/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:44:03 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/16 09:48:19 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit_pipe(t_token *head, t_list **envl, t_list **exp_list,
		char **paths)
{
	long	val;

	if (!head->args[1])
	{
		clear_child(head, envl, exp_list, paths);
		exit(g_glo.sts);
	}
	val = ft_atoi(head->args[1]);
	if ((!val && head->args[1][0] != '0') || (!val && head->args[1][0] == '-')
		|| val == __LONG_MAX__)
	{
		write(2, "minishell: exit: numeric argument required\n", 44);
		clear_child(head, envl, exp_list, paths);
		exit(2);
	}
	else if (head->args[2])
	{
		write(2, "exit\nminishell: exit: too many arguments\n", 42);
		return ;
	}
	else if (val != 0)
	{
		clear_child(head, envl, exp_list, paths);
		exit(val % 256);
	}
}

void	free_v_n(char *v_n, int flag)
{
	if (flag)
		free(v_n);
}
