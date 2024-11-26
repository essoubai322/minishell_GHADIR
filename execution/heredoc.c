/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 05:27:46 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/26 05:28:53 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ran_file(void)
{
	int		fd;
	char	*s;
	char	*tmp;
	int		i[2];
	char	*buff;

	i[0] = 0;
	i[1] = 0;
	buff = malloc(100 * sizeof(char));
	s = malloc(11 * sizeof(char));
	fd = open("/dev/random", O_RDONLY);
	read(fd, buff, 100);
	while (i[0] < 100)
	{
		if (ft_isalnum(buff[i[0]]) && i[1] < 10)
			s[i[1]++] = buff[i[0]];
		i[0]++;
	}
	s[i[1]] = '\0';
	tmp = s;
	s = ft_strjoin("/tmp/", s);
	free(tmp);
	free(buff);
	close(fd);
	return (s);
}

int	ff_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (1);
	return (0);
}

int	is_q(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	heredoc(t_token *head, t_list *list[2], t_token	*tmp, int status)
{
	char	*file_name;
	char	fn[16];

	tmp = head;
	while (head)
	{
		signal_setup(0);
		if (head->type == HEREDOC)
		{
			file_name = ran_file();
			ft_strlcpy(fn, file_name, 16);
			if (fork() == 0)
			{
				free(file_name);
				fork_heredoc(fn, head, list, tmp);
			}
			wait(&status);
			free_arr(head->args);
			free_re(head, file_name);
			if (WEXITSTATUS(status) == 5)
				return (g_glo.sts = 130, 0);
		}
		head = head->next;
	}
	return (1);
}
