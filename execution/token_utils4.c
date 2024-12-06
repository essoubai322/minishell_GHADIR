/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 06:10:09 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/06 21:21:47 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*string_command(const char *input, int *i)
{
	char	*result;
	int		k;

	result = ft_calloc(10000000, sizeof(char));
	k = 0;
	while (isspace(input[*i]))
		(*i)++;
	while (input[*i] && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			g_glo.quote = input[*i];
			parse_quoted(input, i, result, &k);
		}
		else
			parse_unquoted(input, i, result, &k);
	}
	result[k] = '\0';
	return (result);
}

void	skip_whitespace(const char *input, int *i)
{
	while (isspace(input[*i]))
		(*i)++;
}

int	parse_single_quote(const char *input, int *i, char *result, int *k)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
		result[(*k)++] = input[(*i)++];
	if (input[*i])
		(*i)++;
	return (1);
}

int	parse_double_quote(const char *input, int *i, char *result, int *k)
{
	(*i)++;
	while (input[*i] && input[*i] != '"')
		result[(*k)++] = input[(*i)++];
	if (input[*i])
		(*i)++;
	return (1);
}

void	parse_unquoted_heredoc1(const char *input, int *i, char *result, int *k)
{
	while (input[*i] && !isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
	{
		result[(*k)++] = input[(*i)++];
	}
}
