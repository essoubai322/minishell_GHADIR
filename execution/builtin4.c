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

void	free_re(t_token *head, char *file_name)
{
	head->args = malloc(sizeof(char *) * 2);
	head->args[0] = ft_strdup("<");
	head->args[1] = NULL;
	head->type = RED;
	head->arg_size = 1;
	free(head->next->args[0]);
	head->next->args[0] = file_name;
	head->next->type = FILE_N;
}

char	*handle_single_quote(char **str, int *i, char *tmp, int *j)
{
	(*i)++;
	while ((*str)[*i] && (*str)[*i] != '\'')
		tmp[(*j)++] = (*str)[(*i)++];
	(*i)++;
	return (tmp);
}

char	*handle_double_quote(char **str, int *i, char *tmp, int *j)
{
	(*i)++;
	while ((*str)[*i] && (*str)[*i] != '"')
		tmp[(*j)++] = (*str)[(*i)++];
	(*i)++;
	return (tmp);
}
