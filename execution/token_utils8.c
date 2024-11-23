/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils8.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:03:54 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/23 17:04:18 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	while_in_RED_input(char *input, t_token2 **head)
{
	if (input[global.i + 1] == '<')
	{
		add_token(head, HEREDOC1, "<<");
		global.i += 2;
		global.apa = after_heredoc1(input, &global.i);
		if (global.apa)
			add_token_v2(head, WORD, global.apa);
		free(global.apa);
	}
	else
	{
		add_token(head, REDIRECT_IN, "<");
		global.i++;
	}
}

void	while_in_RED_output(char *input, t_token2 **head)
{
	if (input[global.i + 1] == '>')
	{
		add_token(head, REDIRECT_APPEND, ">>");
		global.i += 2;
	}
	else
	{
		add_token(head, REDIRECT_OUT, ">");
		global.i++;
	}
}

void	while_in_quote_dquote(char *input, int f)
{
	if (!f)
	{
		global.in_quote = 1;
		global.current_token[global.current_token_length++] = input[global.i];
		global.i++;
	}
	else
	{
		global.in_dquote = 1;
		global.current_token[global.current_token_length++] = input[global.i];
		global.i++;
	}
}

void	while_in_check_RED_HEREDOC(char *input, t_token2 **head)
{
	if (input[global.i] == '|')
	{
		add_token(head, PIPE1, "|");
		global.i++;
	}
	else if (input[global.i] == '<')
		while_in_RED_input(input, head);
	else if (input[global.i] == '>')
		while_in_RED_output(input, head);
	else if (input[global.i] == '\'')
		while_in_quote_dquote(input, 0);
	else if (input[global.i] == '"')
		while_in_quote_dquote(input, 1);
	else if (isspace(input[global.i]))
		global.i++;
	else if ((input[global.i - 1] == '\'' || input[global.i - 1] == '"')
			&& input[global.i])
	{
		global.current_token[global.current_token_length++] = input[global.i];
		global.i++;
	}
}

void	in_expand_variable(char *input, char *var_name, char **env_value, int k)
{
	if (ft_strncmp(input + global.i, "?", 1) == 0)
	{
		global.i++;
		*env_value = ft_itoa(global.sts);
	}
	else
	{
		while (isalnum(input[global.i]) || input[global.i] == '_')
			var_name[k++] = input[global.i++];
		var_name[k] = '\0';
		if (k == 0)
		{
			*env_value = ft_calloc(2, sizeof(char));
			*env_value[0] = input[global.i - 1];
		}
		else
			*env_value = ft_get_env(var_name, global.env);
	}
}