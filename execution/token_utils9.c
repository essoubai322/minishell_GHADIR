/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils9.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:05:18 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/23 19:33:40 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	while_in_expand_variable(char *input)
{
	char	*env_value;
	char	*var_name;
	int		k;

	;
	k = 0;
	g_glo.i++;
	env_value = NULL;
	var_name = ft_calloc(ft_strlen(input), sizeof(char *));
	in_expand_variable(input, var_name, &env_value, k);
	if (env_value)
	{
		ft_strcat_stack(g_glo.current_token, env_value);
		g_glo.current_token_length += ft_strlen(env_value);
	}
	else if (input[g_glo.i - 1] == '\'')
	{
		g_glo.current_token[g_glo.current_token_length++] = '$';
		ft_strcat_stack(g_glo.current_token, var_name);
		g_glo.current_token_length += ft_strlen(var_name);
	}
	free(env_value);
	free(var_name);
}

void	while_in_expand_variables(char *input)
{
	if (input[g_glo.i] == '$' && g_glo.in_dquote == 1)
	{
		while_in_expand_variable(input);
	}
	else
	{
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i];
		g_glo.i++;
	}
}

void	while_in_check_quote_dquote(char *input, t_token2 **head)
{
	if (input[g_glo.i] == '|' || input[g_glo.i] == '<'
		|| input[g_glo.i] == '>' || input[g_glo.i] == '\''
		|| input[g_glo.i] == '"' || isspace(input[g_glo.i]) || (g_glo.i > 0
		&& (input[g_glo.i - 1] == '\'' || input[g_glo.i - 1] == '"')
		&& input[g_glo.i]))
	{
		while_in_current_bigger_than_zero(input, head);
		while_in_check_RED_HEREDOC(input, head);
	}
	else
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i++];
}

void	while_loop(char *input, t_token2 **head)
{
	if (check_case(input, g_glo.i) && !g_glo.current_token_length)
	{
		if (while_in_string_command(input, head))
			return ;
	}
	else if (!g_glo.in_quote && !g_glo.in_dquote)
	{
		while_in_check_quote_dquote(input, head);
	}
	else if (g_glo.in_quote == 1 && input[g_glo.i] == '\'')
	{
		g_glo.in_quote = 0;
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i++];
	}
	else if (g_glo.in_dquote == 1 && input[g_glo.i] == '"')
	{
		g_glo.in_dquote = 0;
		g_glo.current_token[g_glo.current_token_length++] = input[g_glo.i++];
	}
	else
		while_in_expand_variables(input);
}

void	check_syntax_error_v1(t_token2 *current, char **error)
{
	if (g_glo.in_quote || g_glo.in_dquote)
		*error = ft_strdup("bash: unclosed quote detected");
	while (current != NULL && current->next != NULL)
	{
		if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT
				|| current->type == REDIRECT_APPEND
				|| current->type == HEREDOC1) && ((current->next->type != WORD)
				&& (current->next->type != DQUOTE)
				&& (current->next->type = QUOTE)))
		{
			*error = ft_strdup("bash: syntax error near unexpected token");
			break ;
		}
		current = current->next;
	}
}