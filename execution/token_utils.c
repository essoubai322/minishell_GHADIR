/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:13:57 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/22 20:07:55 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void inialize_global(void)
{
    global.i = 0;
    global.current_token = NULL;
    global.current_token_length = 0;
    global.in_quote = 0;
    global.in_dquote = 0;
    global.error_message = NULL;
    global.apa = NULL;
    global.str_cmd = NULL;
    global.k = 0;
    global.stripped_value = NULL;
    global.env_value = NULL;
    global.o = 0;
}

char *ft_strcat(char *dest, char *src)
{
	int i = 0;
	int j = 0;
	char *result = calloc(ft_strlen(dest) + ft_strlen(src) + 1, sizeof(char));
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

void ft_free2(char ***dest)
{
    if (!dest || !*dest)
        return;

    size_t i = 0;
    while ((*dest)[i])
    {
        free((*dest)[i]);
        (*dest)[i] = NULL;
        i++;
    }
    free(*dest);
    *dest = NULL;
}


t_token2 *create_token(enum TokenType type, const char *value)
{
	t_token2 *new_token = malloc(sizeof(t_token));
	if (!new_token)
        return NULL;	
	new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return (new_token);
}

void    add_token_env_value(char *env_value, char **stripped_value, char *value, int *i)
{
    if (value[*i] == '$' || ft_strchr(value + *i, '$'))
    {
        char *temp = *stripped_value;
        *stripped_value = ft_strcat(*stripped_value, env_value);
        free(temp);
    }
    else
    {
        if (ft_strchr(env_value, ' '))
        {
            char *env_value1 = ft_strcat(env_value, value + *i);
            char *temp = *stripped_value;
            *stripped_value = ft_strcat(*stripped_value, env_value1);
            free(temp);
            free(env_value1);
        }
        else
        {
            char *temp = *stripped_value;
            *stripped_value = ft_strcat(*stripped_value, env_value);
            free(temp);
        }
    }
    free(env_value);
}

