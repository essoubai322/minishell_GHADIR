/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokenv2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 20:22:01 by asebaai           #+#    #+#             */
/*   Updated: 2024/09/30 20:22:22 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar_sign(t_tokenizer *tz, char *value, t_token **head,
		enum TokenType type)
{
	handle_env_var(tz, value);
	if (process_arg_space(tz, head, type))
		return ;
}

void	handle_non_dollar_sign(t_tokenizer *tz, char *value)
{
	char	*var_name;

	var_name = calloc(ft_strlen(value), sizeof(char));
	while (value[tz->i] && value[tz->i] != '$')
		var_name[tz->k++] = value[tz->i++];
	var_name[tz->k] = '\0';
	tz->k = 0;
	if (var_name)
		strcat(tz->stripped_value, var_name);
	free(var_name);
}

void	handle_word_with_dollar(t_tokenizer *tz, char *value, t_token **head,
		enum TokenType type)
{
	while (value[tz->i] != '\0')
	{
		if (value[tz->i] == '$')
			handle_dollar_sign(tz, value, head, type);
		else
			handle_non_dollar_sign(tz, value);
	}
}

void	handle_word_with_dollar_middle(t_tokenizer *tz, char *value,
		t_token **head, enum TokenType type)
{
	char	*name;

	name = calloc(ft_strlen(value), sizeof(char));
	while (value[tz->i] && value[tz->i] != '$')
		name[tz->k++] = value[tz->i++];
	name[tz->k] = '\0';
	tz->k = 0;
	strcat(tz->stripped_value, name);
	free(name);
	while (value[tz->i] != '\0')
	{
		if (value[tz->i] == '$')
			handle_dollar_sign(tz, value, head, type);
		else
			handle_non_dollar_sign(tz, value);
	}
}

void	handle_word(t_tokenizer *tz, char *value, t_token **head,
		enum TokenType type)
{
	if (value && value[0] == '$')
		handle_word_with_dollar(tz, value, head, type);
	else if (strchr(value, '$') != 0)
		handle_word_with_dollar_middle(tz, value, head, type);
	else
		strcpy(tz->stripped_value, value);
}
