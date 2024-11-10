/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:47:15 by rbenmakh          #+#    #+#             */
/*   Updated: 2024/09/10 09:47:46 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_up_env_exp(t_list **envl, t_list **exp_list, char **env)
{
	char	*tmp;
	char	*val;

	*envl = setup_env(env);
	*exp_list = setup_exp(*envl);
	if (*envl)
	{
		val = ft_itoa(ft_atoi(tmp = get_var("SHLVL", *envl)) + 1);
		free(tmp);
		export(exp_list, envl, "SHLVL=", val);
		free(val);
	}
	if (*envl == NULL)
	{
		export(exp_list, envl, "PWD=", tmp = getcwd(NULL, 0));
		export(exp_list, envl, "SHLVL=", "1");
		export(exp_list, envl, "_=", "/usr/bin/env");
		free(tmp);
	}
}

void	cleanup_run_cmd(char ***env, char **cmd, char **paths, t_token *head)
{
	free_run_cmd(paths, *env, env, 0);
	free_str(cmd, &head->args[0]);
}

void	setup_lists(t_list **lists[2], t_list **envl, t_list **exp_list)
{
	lists[0] = envl;
	lists[1] = exp_list;
}
