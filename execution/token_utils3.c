/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:19:35 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/06 21:26:09 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_case(char *input, int i)
{
	char	*result;
	int		k;

	result = NULL;
	k = 0;
	result = check_case_v1(&input, &i, &k, result);
	if (ft_strchr(result, '`'))
	{
		if (counter_closes(result) == 2)
		{
			if (result[0] != '`' || result[k - 1] != '`')
				return (free(result), 1);
		}
		else if (counter_closes(result) && counter_closes(result) % 2 == 0)
			return (free(result), 1);
	}
	free(result);
	return (0);
}

int	expand_cat(char *env_value, char *result, int arr[3])
{
	char	**expanded_tokens;
	int		s;

	s = 0;
	if (env_value)
	{
		expanded_tokens = ft_split(env_value, ' ');
		while (expanded_tokens[s])
		{
			ft_strcat_stack(result, expanded_tokens[s]);
			if (expanded_tokens[s + 1] && !arr[2])
				ft_strcat_stack(result, " ");
			else if (expanded_tokens[s + 1] && arr[2])
				ft_strcat_stack(result, "`");
			s++;
		}
		ft_free2(&expanded_tokens);
	}
	return (arr[2]);
}

int	expand_variable(const char *input, char *result, int *arr, char *env_value)
{
	char	*var_name;

	var_name = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	(arr[0])++;
	if (ft_strncmp(input + arr[0], "?", 1) == 0)
	{
		arr[0]++;
		env_value = ft_itoa(g_glo.sts);
	}
	else
	{
		while (input[arr[0]] && (isalnum(input[arr[0]])
				|| input[arr[0]] == '_'))
			var_name[arr[3]++] = input[(arr[0])++];
		var_name[arr[3]] = '\0';
		if (arr[3] == 0)
		{
			env_value = ft_calloc(2, sizeof(char));
			env_value[0] = input[arr[0] - 1];
		}
		else
			env_value = ft_get_env(var_name, g_glo.env);
	}
	return ((arr[2] = expand_cat(env_value, result, arr), free(env_value), free
			(var_name), arr[1] = ft_strlen(result)), arr[3]);
}

void	parse_quoted(const char *input, int *i, char *result, int *k)
{
	int		arr[4];
	char	*tmp;

	(*i)++;
	tmp = NULL;
	arr[0] = *i;
	arr[1] = *k;
	arr[2] = 0;
	arr[3] = 0;
	while (input[arr[0]] && input[arr[0]] != g_glo.quote)
	{
		if (g_glo.quote == '"' && input[arr[0]] == '$')
			expand_variable(input, result, arr, tmp);
		else
			result[(arr[1])++] = input[(arr[0])++];
		*i = arr[0];
		*k = arr[1];
	}
	if (input[arr[0]])
		(arr[0])++;
	*i = arr[0];
	*k = arr[1];
}

void	parse_unquoted(const char *input, int *i, char *result, int *k)
{
	int		arr[4];
	char	*tmp;

	tmp = NULL;
	arr[0] = *i;
	arr[1] = *k;
	arr[2] = 1;
	arr[3] = 0;
	while (input[arr[0]] && !isspace(input[arr[0]]) && input[arr[0]] != '\''
		&& input[arr[0]] != '"' && input[arr[0]] != '|' && input[arr[0]] != '>'
		&& input[arr[0]] != '<')
	{
		if (input[*i] == '$')
			expand_variable(input, result, arr, tmp);
		else
			result[arr[1]++] = input[arr[0]++];
		*i = arr[0];
		*k = arr[1];
	}
	*i = arr[0];
	*k = arr[1];
}
