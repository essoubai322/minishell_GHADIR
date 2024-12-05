/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 18:26:18 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/16 09:49:48 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*last_io(t_token *head, int type)
{
	char	*file;

	file = NULL;
	while (head)
	{
		if (head->args[0] && head->args[0][0] == '<' && type)
		{
			if (head->next->args[0] && (!ft_strncmp(head->next->args[0],
						"/dev/stdin", 11) || !ft_strncmp(head->next->args[0],
						"/dev/stdout ", 13)))
			{
				head = head->next;
				continue ;
			}
			file = head->next->args[0];
		}
		else if (head->args[0] && head->args[0][0] == '>' && !type)
			file = head->next->args[0];
		head = head->next;
	}
	return (file);
}

int	redir_output(char *filename, int flag)
{
	int		fd;
	char	**arg_space;
	int		s;

	arg_space = ft_split(filename, ' ');
	s = 0;
	fd = 0;
	while (arg_space && arg_space[s])
		s++;
	ft_free2(&arg_space);
	if (flag == 1 && s == 1)
		fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else if (s <= 1)
		fd = open(filename, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (s > 1 || dup2(fd, 1) == -1)
	{
		write(2, "minishell: Ambiguous redirect or permission denied\n", 52);
		g_glo.sts = 1;
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	warning_input(int s, int fd)
{
	if (s > 1 || s == 0)
	{
		write(2, "minishell: Ambiguous redirect\n", 31);
		g_glo.sts = 1;
		if (fd != -1)
			close(fd);
		return (-1);
	}
	else if (fd == -1)
	{
		write(2, "minishell: no such file or directory\n", 38);
		g_glo.sts = 1;
		return (-1);
	}
	return (0);
}

int	redir_input(char *filename)
{
	int		fd;
	char	**arg_space;
	int		s;

	arg_space = ft_split(filename, ' ');
	s = 0;
	fd = 0;
	while (arg_space && arg_space[s])
		s++;
	ft_free2(&arg_space);
	if (!access(filename, F_OK) && access(filename, F_OK | R_OK) == -1)
	{
		write(2, "minishell: Permission denied\n", 29);
		g_glo.sts = 1;
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (warning_input(s, fd) == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

void	while_redir(t_token *head, int *flag, int r)
{
	t_token	*tmp;
	int		ret;

	tmp = head;
	while (tmp)
	{
		if (tmp->args[0][0] == '<'
			&& check_input_redirection(tmp->next->args[0]))
		{
			break ;
		}
		else if (tmp->args[0][0] == '>')
		{
			(void)r;
			ret = redir_output(tmp->next->args[0], check_redir(tmp, 0));
			if (ret == -1)
			{
				*flag += (ret == -1) * -1;
				break ;
			}
		}
		tmp = tmp->next;
	}
}
