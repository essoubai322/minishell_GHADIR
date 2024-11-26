/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_v2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 05:30:03 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/26 06:43:54 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_int(int *i)
{
	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
}

int	read_put(char *file_name, char *del, int q, t_list *env)
{
	char	*str;
	int		i[3];
	int		fd;

	fd = open(file_name, O_CREAT | O_RDWR, 0777);
	while (1)
	{
		str = readline(">");
		if (!str)
		{
			write(1, "minishell:warning: here-doc delimited by EOF!\n", 47);
			ft_lstclear(&env, free);
			return (1);
		}
		if (ff_strncmp(str, del, ft_strlen(del)) || (ft_strlen(str) == 0
				&& ft_strlen(del) == 0))
			break ;
		set_int(i);
		if (q == 0)
			// str = expand_h(str, env, i, NULL);
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
	return (close(fd), 0);
}

// create function to remove quotes rm_quote(&head->next->args[0]) and without problem heap-buffer-overflow

void rm_quote(char **str)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = calloc(sizeof(char) , (ft_strlen(*str) + 1));
	while ((*str)[i])
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
			i++;
		else
			tmp[j++] = (*str)[i++];
	}
	tmp[j] = '\0';
	free(*str);
	*str = tmp;
}

void	fork_heredoc(char *fn, t_token *head, t_list *list[2], t_token	*tmp)
{
	signal_setup(1);
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

void	free_re(t_token *head, char *file_name)
{
	head->args = malloc(sizeof(char *) * 2);
	head->args[0] = ft_strdup("<");
	head->args[1] = NULL;
	head->type = RED;
	head->arg_size = 1;
	free(head->next->args[0]);
	head->next->args[0] = file_name;
	head->next->type = FILE_N;
}

// char	*expand_h(char *str, t_list *env, int *q, char ***temp)
// {
// 	while (str && str[0] && str[q[2]])
// 	{
// 		if (str[q[2]] == '$' && str[q[2] + 1]
// 			&& !ft_strchr("\\#=+-^[].,!@;*%~&{}() \t", str[q[2] + 1])
// 			&& !(ft_strchr("\"", str[q[2] + 1]) && q[1]))
// 			return (expand_h(vars_sub(str, q[2] + 1, env), env, q, temp));
// 		q[2]++;
// 	}
// 	if (temp)
// 		*temp = NULL;
// 	return (str);
// }
