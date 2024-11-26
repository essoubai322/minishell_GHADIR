/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:18:51 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/25 01:37:36 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token_v0(char *value, int *i, char **stripped_value, int *k)
{
	char	*name;
	char	*temp;

	if (value[0] != '$')
	{
		name = calloc(ft_strlen(value), sizeof(char));
		while (value[*i] && value[*i] != '$')
			name[(*k)++] = value[(*i)++];
		name[*k] = '\0';
		*k = 0;
		temp = *stripped_value;
		*stripped_value = ft_strcat(*stripped_value, name);
		free(temp);
		free(name);
	}
}

void	add_token_v1(t_token2 **head, enum TokenType type)
{
	add_token_innit_head(head, g_glo.sdv, type);
	free(g_glo.sdv);
}

void	add_token(t_token2 **head, enum TokenType type, char *v)
{
	g_glo.sdv = get_stripped_value(type, v, g_glo.sdv);
	if (type == WORD)
	{
		if (v && strchr(v, '$') != 0)
		{
			add_token_v0(v, &g_glo.o, &g_glo.sdv, &g_glo.k);
			while (v[g_glo.o] != '\0')
			{
				if (v[g_glo.o] == '$')
				{
					g_glo.env_value = ft_get_value(&g_glo.o, v);
					add_tev(g_glo.env_value, &g_glo.sdv, v, &g_glo.o);
					if (v[g_glo.o] != '$' && !ft_strchr(v + g_glo.o, '$'))
						if (add_token_check(head, g_glo.sdv, type))
							return ;
				}
				else
					add_token_else(v, &g_glo.o, &g_glo.sdv,
						&g_glo.k);
			}
		}
		else
			strcpy(g_glo.sdv, v);
	}
	add_token_v1(head, type);
}

void	add_token_v2(t_token2 **head, enum TokenType type, const char *value)
{
	t_token2	*new_token;
	t_token2	*current;

	new_token = create_token(type, value);
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

int	counter_closes(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input && input[i])
	{
		if (input[i] == '`')
			count++;
		i++;
	}
	return (count);
}
