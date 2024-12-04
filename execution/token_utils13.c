/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils13.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:32:59 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/04 11:05:32 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_unquoted_section(char **u, int *i, int *k, char *result)
{
	while ((*u)[*i] && !isspace((*u)[*i])
		&& (*u)[*i] != '\'' && (*u)[*i] != '"')
		result[(*k)++] = (*u)[(*i)++];
	return (1);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void	del(void *c)
{
	free(c);
}

int	ft_ncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	if ((i == n || (s1[i] == '\0' && s2[i] == '\0')) && s2[i] == '=')
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	check_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "cd", 3))
		return (0);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (0);
	if (!ft_strncmp(cmd, "export", 7))
		return (0);
	if (!ft_strncmp(cmd, "unset", 6))
		return (0);
	if (!ft_strncmp(cmd, "env", 4))
		return (0);
	if (!ft_strncmp(cmd, "echo", 5))
		return (0);
	if (!ft_strncmp(cmd, "exit", 5))
		return (0);
	return (1);
}
