/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils8.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:03:54 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/01 06:04:49 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	while_in_RED_input(char *input, t_token2 **head)
{
	if (input[g_glo.i + 1] == '<')
	{
		add_token(head, HEREDOC1, "<<");
		g_glo.i += 2;
		g_glo.apa = after_heredoc1(input, &g_glo.i);
		if (g_glo.apa)
			add_token_v2(head, WORD, g_glo.apa);
		free(g_glo.apa);
	}
	else
	{
		add_token(head, REDIRECT_IN, "<");
		g_glo.i++;
	}
}

void	while_in_RED_output(char *input, t_token2 **head)
{
	if (input[g_glo.i + 1] == '>')
	{
		add_token(head, REDIRECT_APPEND, ">>");
		g_glo.i += 2;
	}
	else
	{
		add_token(head, REDIRECT_OUT, ">");
		g_glo.RED = 1;
		g_glo.i++;
	}
}

void	while_in_quote_dquote(char *input, int f)
{
	if (!f)
	{
		g_glo.in_quote = 1;
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i];
		g_glo.i++;
	}
	else
	{
		g_glo.in_dquote = 1;
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i];
		g_glo.i++;
	}
}

void	while_in_check_RED_HEREDOC(char *input, t_token2 **head)
{
	if (input[g_glo.i] == '|')
	{
		add_token(head, PIPE1, "|");
		g_glo.i++;
	}
	else if (input[g_glo.i] == '<')
		while_in_RED_input(input, head);
	else if (input[g_glo.i] == '>')
		while_in_RED_output(input, head);
	else if (input[g_glo.i] == '\'')
		while_in_quote_dquote(input, 0);
	else if (input[g_glo.i] == '"')
		while_in_quote_dquote(input, 1);
	else if (isspace(input[g_glo.i]))
		g_glo.i++;
	else if ((input[g_glo.i - 1] == '\'' || input[g_glo.i - 1] == '"')
			&& input[g_glo.i])
	{
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i];
		g_glo.i++;
	}
}

void	in_expand_variable(char *input, char *var_name, char **env_value, int k)
{
	if (ft_strncmp(input + g_glo.i, "?", 1) == 0)
	{
		g_glo.i++;
		*env_value = ft_itoa(g_glo.sts);
	}
	else
	{
		while (isalnum(input[g_glo.i]) || input[g_glo.i] == '_')
			var_name[k++] = input[g_glo.i++];
		var_name[k] = '\0';
		if (k == 0)
		{
			*env_value = ft_calloc(2, sizeof(char));
			*env_value[0] = input[g_glo.i - 1];
		}
		else
			*env_value = ft_get_env(var_name, g_glo.env);
	}
}