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
