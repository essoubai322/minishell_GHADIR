/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils12.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:54:01 by asebaai           #+#    #+#             */
/*   Updated: 2024/12/07 09:00:54 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_tev(char *env_value, char **stripped_value, char *value,
int *i)
{
	char	*temp;

	if (value[*i] == '$' || ft_strchr(value + *i, '$'))
	{
		temp = *stripped_value;
		*stripped_value = ft_strcat(*stripped_value, env_value);
		free(temp);
	}
	else
	{
		*i = add_token_env_value_part(env_value, stripped_value, value, *i);
	}
	free(env_value);
}

int	ft_get_endo(char *value, int i, char **env_value, char *var_name)
{
	if (ft_strncmp(value + i, "?", 1) == 0)
	{
		i++;
		*env_value = ft_itoa(g_glo.sts);
		free(var_name);
		return (1);
	}
	return (0);
}

t_token	*create_and_init_token(const char *value, t_type new_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = new_type;
	if (new_type == RED && strcmp(value, ">>") == 0)
		new_token->arg_size = 2;
	else
		new_token->arg_size = 1;
	new_token->args = malloc(sizeof(char *) * 2);
	if (!new_token->args)
	{
		free(new_token);
		return (NULL);
	}
	new_token->args[0] = ft_strdup(value);
	new_token->args[1] = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	unset_v2(t_list **envl, char **var, int flag)
{
	char	*var_name;
	int		i;

	var_name = NULL;
	i = 1;
	while (var[i] && var[i][0] != '\0')
	{
		var_name = ft_strjoin(var[i], "=");
		if (unset_errors(var_name, flag))
			return ;
		unset(envl, var_name, flag);
		free(var_name);
		i++;
	}
}

void	redirection(t_token *head, t_list **envl, t_list **exp_list)
{
	int		old_fd[2];
	char	*input;
	int		r;
	int		flag;

	flag = 0;
	input = last_io(head, 1);
	old_fd[0] = dup(STDIN_FILENO);
	old_fd[1] = dup(STDOUT_FILENO);
	if (input)
		flag = redir_input(input);
	r = check_redir(head, 0);
	if (r)
		while_redir(head, &flag, r);
	if (flag != -1)
		run_cmd(head, envl, exp_list, split_paths(get_path(*envl)));
	dup2(old_fd[0], STDIN_FILENO);
	dup2(old_fd[1], STDOUT_FILENO);
	close(old_fd[0]);
	close(old_fd[1]);
}
