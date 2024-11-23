/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils9.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:05:18 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/23 17:05:44 by asebaai          ###   ########.fr       */
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
	global.i++;
	env_value = NULL;
	var_name = ft_calloc(ft_strlen(input), sizeof(char *));
	in_expand_variable(input, var_name, &env_value, k);
	if (env_value)
	{
		ft_strcat_stack(global.current_token, env_value);
		global.current_token_length += ft_strlen(env_value);
	}
	else if (input[global.i - 1] == '\'')
	{
		global.current_token[global.current_token_length++] = '$';
		ft_strcat_stack(global.current_token, var_name);
		global.current_token_length += ft_strlen(var_name);
	}
	free(env_value);
	free(var_name);
}

void	while_in_expand_variables(char *input)
{
	if (input[global.i] == '$' && global.in_dquote == 1)
	{
		while_in_expand_variable(input);
	}
	else
	{
		global.current_token[global.current_token_length++] = input[global.i];
		global.i++;
	}
}

void	while_in_check_quote_dquote(char *input, t_token2 **head)
{
	if (input[global.i] == '|' || input[global.i] == '<'
		|| input[global.i] == '>' || input[global.i] == '\''
		|| input[global.i] == '"' || isspace(input[global.i]) || (global.i > 0
		&& (input[global.i - 1] == '\'' || input[global.i - 1] == '"')
		&& input[global.i]))
	{
		while_in_current_bigger_than_zero(input, head);
		while_in_check_RED_HEREDOC(input, head);
	}
	else
		global.current_token[global.current_token_length++] = input[global.i++];
}

void	while_loop(char *input, t_token2 **head)
{
	if (check_case(input, global.i) && !global.current_token_length)
	{
		if (while_in_string_command(input, head))
			return ;
	}
	else if (!global.in_quote && !global.in_dquote)
	{
		while_in_check_quote_dquote(input, head);
	}
	else if (global.in_quote == 1 && input[global.i] == '\'')
	{
		global.in_quote = 0;
		global.current_token[global.current_token_length++] = input[global.i++];
	}
	else if (global.in_dquote == 1 && input[global.i] == '"')
	{
		global.in_dquote = 0;
		global.current_token[global.current_token_length++] = input[global.i++];
	}
	else
		while_in_expand_variables(input);
}

void	check_syntax_error_v1(t_token2 *current, char **error)
{
	if (global.in_quote || global.in_dquote)
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