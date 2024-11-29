/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils12.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:54:01 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/29 20:06:03 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_tev(char *env_value, char **stripped_value, char *value,
int *i)
{
	char	*temp;
	
	if (value[*i] == '$' || ft_strchr(value + *i, '$'))
	{
		temp = *stripped_value;
		*stripped_value = ft_strcat(*stripped_value, env_value);
		free(temp);
	}
	else
	{
		*i = add_token_env_value_part(env_value, stripped_value, value, *i);
	}
	free(env_value);
}

int ft_get_endo(char *value, int i, char **env_value, char * var_name)
{
	if (ft_strncmp(value + i, "?", 1) == 0)
	{
		i++;
		*env_value = ft_itoa(g_glo.sts);
		free(var_name);
		return (1);
	}
	return (0);
}

t_token	*create_and_init_token(const char *value, t_type new_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = new_type;
	new_token->arg_size = 1;
	new_token->args = malloc(sizeof(char *) * 2);
	if (!new_token->args)
	{
		free(new_token);
		return (NULL);
	}
	new_token->args[0] = ft_strdup(value);
	new_token->args[1] = NULL;
	new_token->next = NULL;
	return (new_token);
}
