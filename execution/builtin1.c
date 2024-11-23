/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:45:49 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/16 09:48:14 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	plus_equal(int *flag, char **var_name)
{
	char	*tmp;

	*var_name = ft_strtrim(*var_name, "+=");
	tmp = *var_name;
	*var_name = ft_strjoin(*var_name, "=");
	free(tmp);
	*flag = 1;
}

int	search_var_replace(t_list **list, char *v_n, char *val, int flag)
{
	char	*tmp[2];
	t_list	*tmpl;

	flag = 0;
	tmpl = *list;
	if (v_n && ft_strchr(v_n, '+'))
		plus_equal(&flag, &v_n);
	while (tmpl)
	{
		tmp[1] = (char *)tmpl->content;
		if (v_n && !val && ft_strnstr(tmp[1], v_n, ft_strlen(v_n)))
			return (1);
		else if (v_n && val && ft_strnstr(tmp[1], v_n, ft_strlen(v_n)) && flag)
			return (change_var_and_free(v_n, val, tmp[1], tmpl), 1);
		else if (v_n && val && ft_strnstr(tmp[1], v_n, ft_strlen(v_n)) && !flag)
		{
			tmp[0] = tmp[1];
			tmpl->is_equal = 0;
			tmpl->content = ft_strjoin(v_n, val);
			return (free(tmp[0]), 1);
		}
		tmpl = tmpl->next;
	}
	return (free_v_n(v_n, flag), 0);
}

int	export_errors(char *var_name)
{
	if (var_name && var_name[0] == '-')
	{
		write(2, "export: bad option: - \n", 24);
		return (1);
	}
	if (var_name && (!var_name[0] || ft_isdigit(var_name[0])
			|| (!ft_isalpha(var_name[0]) && var_name[0] != '_')
			|| (check_var(var_name + 1))))
	{
		write(2, "export: not a valid identifier\n", 32);
		return (1);
	}
	return (0);
}

void	plus_export(char **var_name, int *flag)
{
	char	*tmp;

	*var_name = ft_strtrim(*var_name, "+=");
	tmp = *var_name;
	*var_name = ft_strjoin(*var_name, "=");
	free(tmp);
	*flag = 1;
}

void	export(t_list **exp_list, t_list **envl, char *var_name,
		char *var_value)
{
	int	flags[3];

	flags[2] = 0;
	global.sts = export_errors(var_name);
	if (global.sts)
		return ;
	flags[0] = search_var_replace(envl, var_name, var_value, 0);
	flags[1] = search_var_replace(exp_list, var_name, var_value, 0);
	if (var_name && ft_strchr(var_name, '+'))
		plus_export(&var_name, &flags[2]);
	if (!flags[1] && !var_value && var_name)
		ft_lstadd_back(exp_list, ft_lstnew(ft_strjoin(var_name, "="), 1));
	if (!flags[0] && var_name && var_value)
		ft_lstadd_back(envl, ft_lstnew(ft_strjoin(var_name, var_value), 0));
	if (!flags[1] && var_name && var_value)
		ft_lstadd_back(exp_list, ft_lstnew(ft_strjoin(var_name, var_value), 0));
	if (!var_name)
		print_export(*exp_list);
	if (flags[2])
		free(var_name);
}
