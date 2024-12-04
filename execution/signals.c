/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 14:14:44 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/11 19:58:14 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_glo.sts = 130;
	}
}

void	sig_exit(int num)
{
	if (g_glo.fd_herdoc)
	{
		ft_lstclear(&g_glo.list[0], free);
		ft_lstclear(&g_glo.list[1], free);
		close(g_glo.fd_herdoc);
		g_glo.fd_herdoc = 0;
	}
	(void)num;
	exit(5);
}

void	signal_setup(int n)
{
	if (n == 1)
	{
		signal(SIGINT, sig_exit);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (n == 2)
	{
		signal(SIGINT, sighandler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (n == 3)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	signal_heredoc(t_list *list[2])
{
	g_glo.list[0] = list[0];
	g_glo.list[1] = list[1];
	signal(SIGINT, sig_exit);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_quoted_section(char **u, int *i, int *k, char *result)
{
	char	quote;

	quote = (*u)[*i];
	result[(*k)++] = '`';
	(*i)++;
	while ((*u)[*i] && (*u)[*i] != quote)
		result[(*k)++] = (*u)[(*i)++];
	if (!(*u)[*i])
		return (0);
	result[(*k)++] = '`';
	(*i)++;
	return (1);
}
