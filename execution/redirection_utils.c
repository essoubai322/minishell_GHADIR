/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:49:01 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/16 16:11:07 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_input_redirection(char *filename)
{
	if (access(filename, F_OK))
		return (1);
	else if (!access(filename, F_OK) && access(filename, F_OK | R_OK) == -1)
	{
		return (1);
	}
	return (0);
}

int	check_while_input_redirection(t_token *list)
{
	char	*filename;

	while (list && list->args)
	{
		if (list->args[0] && list->args[0][0] == '<')
		{
			filename = list->next->args[0];
			if (!filename || !filename[0])
			{
				list = list->next;
				continue ;
			}
			if (access(filename, F_OK))
				return (1);
			else if (!access(filename, F_OK) && access(
					filename, F_OK | R_OK) == -1)
			{
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}

void	check_invalid_redirection(t_token *head, t_list **envl,
		t_list **exp_list, char **paths)
{
	int	ret;

	ret = check_while_input_redirection(head);
	if (ret)
	{
		clear_child(head, envl, exp_list, paths);
		exit(1);
	}
}

void	print_and_exit(char *msg, int status)
{
	write(2, msg, ft_strlen(msg));
	exit(status);
}

char	*check_case_v1(char **u, int *i, int *k, char *result)
{
	result = ft_calloc(strlen(*u) + 1, sizeof(char));
	while (isspace((*u)[*i]))
		(*i)++;
	return (handle_token_parsing(u, i, k, result));
}
