/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:18:04 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/06 23:20:26 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token_innit_head(t_token2 **head, char *stripped_value,
		enum e_t type)
{
	t_token2	*new_token;
	t_token2	*current;

	if (stripped_value == NULL)
		return ;
	new_token = create_token(type, stripped_value);
	if (*head == NULL)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_token;
	}
}

int	add_token_check(t_token2 **head, char *stripped_value, enum e_t type)
{
	char	**arg_space;
	int		s;

	arg_space = ft_split(stripped_value, ' ');
	s = 0;
	while (arg_space && arg_space[s])
		s++;
	if (g_glo.red)
	{
		add_token_innit_head(head, stripped_value, type);
		g_glo.red = 0;
		s = 1;
		return (ft_free2(&arg_space), free(stripped_value), 1);
	}
	if (s > 1)
	{
		s = 0;
		while (arg_space[s])
		{
			add_token_innit_head(head, arg_space[s], type);
			s++;
		}
		return (ft_free2(&arg_space), free(stripped_value), 1);
	}
	return (ft_free2(&arg_space), 0);
}

char	*ft_get_value(int *i, char *value)
{
	char	*env_value;
	char	*var_name;
	int		k;

	env_value = NULL;
	var_name = ft_calloc(ft_strlen(value) + 1, sizeof(char));
	(*i)++;
	k = 0;
	if (ft_get_endo(value, *i, &env_value, var_name))
		return ((*i)++, env_value);
	while (value[*i] && (isalnum(value[*i]) || value[*i] == '_'))
		var_name[k++] = value[(*i)++];
	var_name[k] = '\0';
	if (k == 0)
	{
		env_value = ft_calloc(2, sizeof(char));
		env_value[0] = value[*i - 1];
		free(var_name);
		return (env_value);
	}
	if (var_name)
		env_value = ft_get_env(var_name, g_glo.env);
	free(var_name);
	return (env_value);
}

void	add_token_else(char *value, int *i, char **stripped_value, int *k)
{
	char	*var_name;
	char	*temp;

	var_name = ft_calloc(ft_strlen(value), sizeof(char));
	while (value[*i] && value[*i] != '$')
	{
		var_name[*k] = value[(*i)++];
		(*k)++;
	}
	*k = 0;
	if (var_name)
	{
		temp = *stripped_value;
		*stripped_value = ft_strcat(*stripped_value, var_name);
		free(temp);
	}
	free(var_name);
}

char	*get_stripped_value(enum e_t type, char *value,
		char *stripped_value)
{
	int	len;

	g_glo.o = 0;
	g_glo.k = 0;
	stripped_value = ft_calloc(1200500, sizeof(char));
	if (type == QUOTE || type == DQUOTE)
	{
		len = ft_strlen(value);
		if (len > 2)
		{
			ft_strncpy(stripped_value, value + 1, len - 2);
			stripped_value[len - 2] = '\0';
		}
		else
			ft_strcpy(stripped_value, "");
	}
	else if (type == WORD)
		return (stripped_value);
	else
		ft_strcpy(stripped_value, value);
	return (stripped_value);
}
