/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:13:57 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/06 21:21:19 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	inialize_global(void)
{
	g_glo.red = 0;
	g_glo.i = 0;
	g_glo.current_token = NULL;
	g_glo.current_token_length = 0;
	g_glo.in_quote = 0;
	g_glo.in_dquote = 0;
	g_glo.error_message = NULL;
	g_glo.apa = NULL;
	g_glo.str_cmd = NULL;
	g_glo.k = 0;
	g_glo.sdv = NULL;
	g_glo.env_value = NULL;
	g_glo.o = 0;
	g_glo.fd_herdoc = 0;
}

char	*ft_strcat(char *dest, char *src)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = ft_calloc(ft_strlen(dest) + ft_strlen(src) + 1, sizeof(char));
	while (dest && dest[i])
	{
		result[i] = dest[i];
		i++;
	}
	while (src && src[j])
	{
		result[i] = src[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}

void	ft_free2(char ***dest)
{
	size_t	i;

	if (!dest || !*dest)
		return ;
	i = 0;
	while ((*dest)[i])
	{
		free((*dest)[i]);
		(*dest)[i] = NULL;
		i++;
	}
	free(*dest);
	*dest = NULL;
}

t_token2	*create_token(enum e_t type, const char *value)
{
	t_token2	*new_token;

	new_token = malloc(sizeof(t_token2));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

int	add_token_env_value_part(char	*env_value, char	**std, char	*value,
	int i)
{
	char	*temp;
	char	*env_value1;

	if (ft_strchr(env_value, ' '))
	{
		env_value1 = ft_strcat(env_value, value + i);
		temp = *std;
		*std = ft_strcat(*std, env_value1);
		free(temp);
		free(env_value1);
	}
	else
	{
		temp = *std;
		*std = ft_strcat(*std, env_value);
		free(temp);
	}
	return (i);
}
