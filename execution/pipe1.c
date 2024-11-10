/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:52:05 by rbenmakh          #+#    #+#             */
/*   Updated: 2024/09/15 12:06:42 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_unused_fd(t_pipe *fdt, int p)
{
	int	i;

	i = 0;
	while (i < p)
	{
		close(fdt[i].fd[0]);
		close(fdt[i].fd[1]);
		i++;
	}
	free(fdt);
}

void	next_cmd(t_token **head)
{
	while (*head)
	{
		if ((*head)->args[0] && (*head)->args[0][0] == '|')
		{
			*head = (*head)->next;
			break ;
		}
		*head = (*head)->next;
	}
}

void	free_fdt(t_pipe *fdt, int p)
{
	(void)p;
	free(fdt);
	fdt = NULL;
}

void	free_and_wait(t_pipe *fdt, int p, int pid)
{
	(void)fdt;
	waitpid(pid, &p, 0);
	while (wait(NULL) > 0)
	{
	}
	exit_status(p);
}

void	init_var_pipe(t_token *head, int i[5])
{
	i[0] = 0;
	i[3] = calc_pipes(head);
}
