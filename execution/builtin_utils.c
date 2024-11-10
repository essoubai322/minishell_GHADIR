/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:56:45 by rbenmakh          #+#    #+#             */
/*   Updated: 2024/09/25 10:32:32 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_list *envl)
{
	while (envl)
	{
		printf("%s\n", (char *)envl->content);
		envl = envl->next;
	}
}

t_list	*setup_env(char **env)
{
	t_list	*envl;
	int		i;

	envl = NULL;
	i = 0;
	while (env[i])
	{
		ft_lstadd_back(&envl, ft_lstnew(ft_substr(env[i], 0, ft_strlen(env[i])),
				0));
		i++;
	}
	return (envl);
}

t_list	*setup_exp(t_list *envl)
{
	t_list	*exp_list;
	char	*str;

	exp_list = NULL;
	while (envl)
	{
		str = (char *)envl->content;
		ft_lstadd_back(&exp_list, ft_lstnew(ft_substr(str, 0, ft_strlen(str)),
				0));
		envl = envl->next;
	}
	return (exp_list);
}

void	init_export(t_token *head, t_list **envl, t_list **exp_list, int i)
{
	char	*var_value;
	char	*var_name;
	char	*f;

	i = 1;
	while (head->args[i])
	{
		f = NULL;
		if (head->args[i])
			f = ft_strchr(head->args[i], '=');
		var_name = NULL;
		var_value = NULL;
		if (!f && head->args[i])
			var_name = ft_substr(head->args[i], 0, ft_strlen(head->args[i]));
		else if (f && head->args[i])
		{
			var_name = ft_substr(head->args[i], 0, f - head->args[i] + 1);
			var_value = ft_strdup(ft_strchr(head->args[i], '=') + 1);
		}
		export(exp_list, envl, var_name, var_value);
		free(var_name);
		free(var_value);
		i++;
	}
	return ;
}

void	print_export(t_list *exp_list)
{
	t_list	*head;
	t_list	*tmp;
	char	*temp;

	head = exp_list;
	while (exp_list && exp_list->next)
	{
		tmp = exp_list->next;
		while (tmp)
		{
			if (ft_strncmp((char *)exp_list->content, (char *)tmp->content,
					ft_strlen((char *)exp_list->content)) > 0)
			{
				temp = (char *)exp_list->content;
				exp_list->content = tmp->content;
				tmp->content = temp;
			}
			tmp = tmp->next;
		}
		exp_list = exp_list->next;
	}
	while_print_export(head);
}
