/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:25:46 by amoubine          #+#    #+#             */
/*   Updated: 2024/12/04 11:12:53 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_global	g_glo;

void	print_error(char *str)
{
	printf("%s\n", str);
	free(str);
}

void	loop_v2(char *input, t_list **lists)
{
	t_lexer	result;
	t_token	*current2;
	t_token	*new_token;
	t_token	*new_head;

	result = tokenize(input);
	current2 = NULL;
	new_token = NULL;
	if (result.error_message)
		print_error(result.error_message);
	else
	{
		new_head = convert_data(result.tokens, current2, new_token);
		free_tokens(result.tokens);
		reorganize_cmd_to_start(&new_head);
		if (heredoc(new_head, lists, 0, 0) == 0)
		{
			list_clear(&new_head);
			write(1, "\n", 1);
			return ;
		}
		if (!new_head)
			return ;
		excution(&new_head, &lists[0], &lists[1]);
	}
}

void	loop_free(char *input, t_list **lists)
{
	rl_clear_history();
	free(input);
	free_list(lists[0]);
	free_list(lists[1]);
	free_arr(g_glo.env);
}

int	loop(char **env)
{
	char	*input;
	t_list	*lists[2];

	set_up_env_exp(&lists[0], &lists[1], env);
	while (1)
	{
		signal_setup(2);
		g_glo.env = convert_to_array_v2(lists[0], g_glo.env);
		input = readline(PURPLE "APA@GOVOS" BOLD "> " RESET);
		if (!input)
		{
			printf("exit\n");
			loop_free(input, lists);
			exit(g_glo.sts);
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		(loop_v2(input, lists), free(input));
	}
	loop_free(input, lists);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	return (loop(env));
}
