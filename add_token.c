/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 20:20:00 by asebaai           #+#    #+#             */
/*   Updated: 2024/09/30 20:22:21 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quote(t_tokenizer *tz, char *value)
{
	int	len;

	len = ft_strlen(value);
	if (len > 2)
	{
		strncpy(tz->stripped_value, value + 1, len - 2);
		tz->stripped_value[len - 2] = '\0';
	}
	else
		strcpy(tz->stripped_value, "");
}

void	handle_env_var(t_tokenizer *tz, char *value)
{
	char	*var_name;
	char	*env_value;

	var_name = calloc(ft_strlen(value) + 1, sizeof(char));
	tz->i++;
	while (value[tz->i] && (isalnum(value[tz->i]) || value[tz->i] == '_'))
		var_name[tz->k++] = value[tz->i++];
	var_name[tz->k] = '\0';
	tz->k = 0;
	env_value = getenv(var_name);
	if (env_value)
		strcat(tz->stripped_value, env_value);
	free(var_name);
}

void	add_new_token(t_token **head, t_token *new_token)
{
	t_token	*current;

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

int	count_arg_space(t_tokenizer *tz)
{
	tz->s = 0;
	while (tz->arg_space[tz->s])
		tz->s++;
	return (tz->s);
}

int	process_arg_space(t_tokenizer *tz, t_token **head, enum TokenType type)
{
	int		count;
	t_token	*new_token;

	tz->arg_space = ft_split(tz->stripped_value, ' ');
	count = count_arg_space(tz);
	if (count > 1)
	{
		free(tz->stripped_value);
		tz->s = 0;
		while (tz->arg_space[tz->s])
		{
			new_token = create_token(type, tz->arg_space[tz->s]);
			add_new_token(head, new_token);
			tz->s++;
		}
		ft_free2(tz->arg_space);
		return (1);
	}
	ft_free2(tz->arg_space);
	return (0);
}
