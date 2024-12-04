/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils14.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:34:32 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/04 11:05:42 by asebaai          ###   ########.fr       */
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
	i = 0;
	while (paths[i] && paths[i] != '=')
		i++;
	first_part = ft_substr(&paths[i + 1], 0, ft_strlen(paths));
	the_paths = ft_split(first_part, ':');
	free(first_part);
	return (the_paths);
}
