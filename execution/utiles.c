/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 11:16:11 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/11 14:23:49 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
