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
