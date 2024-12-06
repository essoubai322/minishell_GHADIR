/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:47:15 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/10 09:47:46 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_var(t_list **list, char *var_name, char *var_value)
{
	char	*tmp;
	char	*str;
	int		flag;
	t_list	*tmpl;

	flag = 0;
	tmpl = *list;
	while (tmpl)
	{
		str = (char *)tmpl->content;
		if (var_name && ft_strnstr(str, var_name, ft_strlen(var_name)))
		{
			flag = 1;
			tmp = str;
			tmpl->content = ft_strjoin(var_name, var_value);
			free(tmp);
		}
		tmpl = tmpl->next;
	}
	return (flag);
}

int	check_var(char *var_name)
{
	int	i;

	i = 0;
	if (!var_name)
		return (1);
	while (var_name[i] && var_name[i + 1])
	{
		if (var_name[i + 1] == '=' && var_name[i + 2] == '\0'
			&& var_name[i] == '+')
		{
			return (0);
		}
		if (!ft_isalnum(var_name[i]) && var_name[i] != '_')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	change_var_and_free(char *var_name, char *var_value, char *str,
		t_list *tmpl)
{
	char	*tmp;

	tmp = str;
	var_value = ft_strjoin(ft_strchr(str, '=') + 1, var_value);
	tmpl->content = ft_strjoin(var_name, var_value);
	free(tmp);
	free(var_name);
	free(var_value);
}

int	unset_errors(char *var_name, int flag)
{
	if (!var_name)
	{
		if (!flag)
			write(2, "unset: not enough arguments\n", 29);
		return (1);
	}
	else if (check_var(var_name))
	{
		if (!flag)
			write(2, "unset: invalid parameter name\n", 31);
		free(var_name);
		return (1);
	}
	return (0);
}

void	unset(t_list **envl, char *var_name, int flag)
{
	t_list	*env;
	t_list	*prev;

	env = *envl;
	prev = NULL;
	(void)flag;
	while (env)
	{
		if (!ft_strncmp((char *)env->content, var_name, ft_strlen(var_name)))
		{
			if (!prev)
				(*envl) = env->next;
			else
				prev->next = env->next;
			free(env->content);
			free(env);
			break ;
		}
		prev = env;
		env = env->next;
	}
}
