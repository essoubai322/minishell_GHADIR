/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils14.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:34:32 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/06 00:18:45 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(t_list *envl)
{
	char	*path;

	path = NULL;
	if (!getenv("PATH"))
		return (DEFAULT_PATH_VALUE);
	while (envl)
	{
		path = ft_strnstr((char *)envl->content, "PATH=", 5);
		if (path)
			break ;
		envl = envl->next;
	}
	return (path);
}

char	**split_paths(char *paths)
{
	int		i;
	char	*first_part;
	char	**the_paths;

	i = 0;
	if (paths == NULL)
		return (NULL);
	while (paths[i] && paths[i] != '=')
		i++;
	first_part = ft_substr(&paths[i + 1], 0, ft_strlen(paths));
	the_paths = ft_split(first_part, ':');
	free(first_part);
	return (the_paths);
}

int	exec_pipes(t_token *list[2], t_list **envl, t_list **exp_list, char **paths)
{
	int		i[5];
	t_pipe	*fdt;

	init_var_pipe(list[0], i);
	fdt = init_pipes(i[3]);
	while (i[0] <= i[3])
	{
		i[1] = fork();
		if (!i[1])
		{
			if (i[0] != i[3])
				dup2(fdt[i[0]].fd[1], STDOUT_FILENO);
			if (i[0] > 0)
				dup2(fdt[i[0] - 1].fd[0], STDIN_FILENO);
			close_unused_fd(fdt, i[3]);
			run(list, envl, exp_list, paths);
		}
		else if (i[0] == i[3])
			i[4] = i[1];
		next_cmd(&list[1]);
		i[0]++;
	}
	close_unused_fd(fdt, i[3]);
	free_and_wait(fdt, i[3], i[4]);
	return (free_arr(paths), 0);
}
