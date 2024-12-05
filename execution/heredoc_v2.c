/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 05:30:03 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/05 10:20:25 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*var_name_heredoc(char *input, int *i)
{
	char	*var_name;
	int		c;

	c = 0;
	var_name = ft_calloc(sizeof(char), 100000);
	if (input[*i - 1] == '$' && ft_strncmp(input + *i, "?", 1) == 0)
	{
		(*i)++;
		var_name = ft_itoa(g_glo.sts);
	}
	else
	{
		while (input[*i] && (isalnum(input[*i])
				|| input[*i] == '_'))
			var_name[c++] = input[(*i)++];
		var_name[c] = '\0';
	}
	return (var_name);
}

void	init_arr(int *arr)
{
	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 0;
}

char	*in_expand_variable_v2(char *s, t_list *env)
{
	char	*tmp;
	int		i[3];

	init_arr(i);
	tmp = ft_calloc(sizeof(char), 100000);
	while (s[i[0]])
	{
		if (s[i[0]] == '$')
			tmp = process_variable(s, env, tmp, i);
		else
			tmp = copy_normal_char(tmp, s, i, &i[1]);
	}
	return (free(s), tmp);
}

int	read_put(char *file_name, char *del, int q, t_list *env)
{
	char	*str;
	int		fd;

	fd = open(file_name, O_CREAT | O_RDWR, 0777);
	g_glo.fd_herdoc = fd;
	while (1)
	{
		str = readline(">");
		if (!str)
		{
			write(1, "minishell:warning: here-doc delimited by EOF!\n", 47);
			ft_lstclear(&env, free);
			return (free(str), close(fd), 1);
		}
		if (ff_strncmp(str, del, ft_strlen(del)) || (ft_strlen(str) == 0
				&& ft_strlen(del) == 0))
			break ;
		if (q == 0)
			str = in_expand_variable_v2(str, env);
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
	return (close(fd), 0);
}

void	fork_heredoc(char *fn, t_token *head, t_list *list[2], t_token	*tmp)
{
	signal_heredoc(list, tmp);
	if (is_q(head->next->args[0]))
	{
		rm_quote(&head->next->args[0]);
		if (read_put(fn, head->next->args[0], 1, list[0]))
		{
			list_clear(&tmp);
			ft_lstclear(&list[1], free);
			exit(0);
		}
	}
	else
	{
		if (read_put(fn, head->next->args[0], 0, list[0]))
		{
			list_clear(&tmp);
			ft_lstclear(&list[1], free);
			exit(0);
		}
	}
	ft_lstclear(&list[0], free);
	ft_lstclear(&list[1], free);
	list_clear(&tmp);
	exit(0);
}
