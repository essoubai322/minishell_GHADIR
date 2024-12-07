/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:46:01 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/16 14:43:04 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_status(int status)
{
	if (WIFEXITED(status))
		g_glo.sts = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_glo.sts = WTERMSIG(status) + 128;
		if (g_glo.sts == 130)
			write(1, "\n", 1);
		if (g_glo.sts == 131)
			write(1, "Quit (core dumped)\n", 20);
	}
}

char	*copy_normal_char(char *tmp, char *s, int *i, int *j)
{
	tmp[*j] = s[i[0]++];
	(*j)++;
	return (tmp);
}

char	*append_var_value(char *tmp, char *var_value, int *i, int *j)
{
	while (var_value[i[2]])
	{
		tmp[*j] = var_value[i[2]++];
		(*j)++;
	}
	i[2] = 0;
	return (tmp);
}

char	*process_variable(char *s, t_list *env, char *tmp, int *i)
{
	char	*var_name;
	char	*var_value;
	int		j;

	j = i[1];
	i[0]++;
	var_name = var_name_heredoc(s, i);
	if (var_name[0] != '$')
		var_value = get_var(var_name, env);
	else
		var_value = ft_strdup("$");
	free(var_name);
	if (var_value)
	{
		tmp = append_var_value(tmp, var_value, i, &j);
		free(var_value);
	}
	i[1] = j;
	return (tmp);
}

char	*rm_quote(char **str)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = ft_calloc(sizeof(char), (ft_strlen(*str) + 1));
	while ((*str)[i])
	{
		if ((*str)[i] == '\'')
			tmp = handle_single_quote(str, &i, tmp, &j);
		else if ((*str)[i] == '"')
			tmp = handle_double_quote(str, &i, tmp, &j);
		else
			tmp[j++] = (*str)[i++];
	}
	free(*str);
	*str = tmp;
	return (tmp);
}
