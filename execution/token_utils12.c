/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils12.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:54:01 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/25 01:26:06 by asebaai          ###   ########.fr       */
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

