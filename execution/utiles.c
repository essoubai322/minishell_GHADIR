/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysahraou <ysahraou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 11:16:11 by ysahraou          #+#    #+#             */
/*   Updated: 2024/09/11 14:23:49 by ysahraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
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

char	*get_var(char *str, t_list *env)
{
	if (str && str[0] == '\0')
		return (NULL);
	while (env)
	{
		if (ft_ncmp(str, env->content, ft_strlen(str)) == 0)
		{
			return (ft_substr(env->content, ft_strlen(str) + 1,
					ft_strlen(env->content)));
		}
		env = env->next;
	}
	return (NULL);
}

void	error_and_exit(int exit_num)
{
	perror("minishell: ");
	exit(exit_num);
}

void	error_func(int errnum, int exit_num)
{
	char	*str_err;

	str_err = strerror(errnum);
	printf("%s\n", str_err);
	(void)exit_num;
}

void	error_exit(char *str, int exit_num)
{
	write(2, str, ft_strlen(str));
	exit(exit_num);
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


void	list_clear(t_token **head)
{
	t_token	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		free_arr((*head)->args);
		(*head)->args = NULL;
		free((*head));
		(*head) = tmp;
	}
	*head = NULL;
}

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
