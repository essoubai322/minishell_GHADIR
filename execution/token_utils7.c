/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils7.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:02:46 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/23 19:33:40 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_list(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->content);
		free(tmp);
	}
}

int	initialize_token(t_token **new_token, t_token *current)
{
	*new_token = malloc(sizeof(t_token));
	if (!*new_token)
		return (1);
	(*new_token)->type = current->type;
	(*new_token)->arg_size = current->arg_size;
	(*new_token)->heredoc = current->heredoc;
	(*new_token)->args = malloc(sizeof(char *) * (current->arg_size + 1));
	if (!(*new_token)->args)
		return (free(new_token), 1);
	return (0);
}

char	*ft_strcat_stack(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

int	while_in_string_command(char *input, t_token2 **head)
{
	char	**arg_space;
	int		s;

	g_glo.str_cmd = string_command(input, &g_glo.i);
	g_glo.apa = ft_strdup(g_glo.str_cmd);
	free(g_glo.str_cmd);
	if (ft_strchr(g_glo.apa, '`'))
	{
		arg_space = ft_split(g_glo.apa, '`');
		s = 0;
		while (arg_space[s])
		{
			add_token_v2(head, WORD, arg_space[s]);
			s++;
		}
		ft_free2(&arg_space);
		free(g_glo.apa);
		return (1);
	}
	add_token_v2(head, WORD, g_glo.apa);
	free(g_glo.apa);
	return (0);
}

void	while_in_current_bigger_than_zero(char *input, t_token2 **head)
{
	if (g_glo.current_token_length > 0)
	{
		if (input[g_glo.i - 1] == '"')
			add_token(head, DQUOTE, g_glo.current_token);
		else if (input[g_glo.i - 1] == '\'')
			add_token(head, QUOTE, g_glo.current_token);
		else
			add_token(head, WORD, g_glo.current_token);
		g_glo.current_token_length = 0;
		ft_memset(g_glo.current_token, 0, ft_strlen(input));
	}
}