/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:25:46 by amoubine          #+#    #+#             */
/*   Updated: 2024/11/26 06:40:01 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

g_global	g_glo;

void	print_tokens_v2(t_token *head)
{
	t_token	*current;

	current = head;
	while (current != NULL)
	{
		if (current->type == CMD)
		{
			printf("CMD : args= ");
			for (int i = 0; i < current->arg_size; i++)
			{
				printf("[%d] '%s'", i, current->args[i]);
				if (i < current->arg_size - 1)
					printf(" ");
			}
			printf("\n");
		}
		else if (current->type == PIPE)
		{
			printf("PIPE : args= ");
			for (int i = 0; i < current->arg_size; i++)
			{
				printf("[%d] '%s'", i, current->args[i]);
				if (i < current->arg_size - 1)
					printf(" ");
			}
			printf("\n");
		}
		else if (current->type == RED)
		{
			printf("RED : args= ");
			for (int i = 0; i < current->arg_size; i++)
			{
				printf("[%d] '%s'", i, current->args[i]);
				if (i < current->arg_size - 1)
					printf(" ");
			}
			printf("\n");
		}
		else if (current->type == HEREDOC)
		{
			printf("HEREDOC : args= ");
			for (int i = 0; i < current->arg_size; i++)
			{
				printf("[%d] '%s'", i, current->args[i]);
				if (i < current->arg_size - 1)
					printf(" ");
			}
			printf("\n");
		}
		else if (current->type == FILE_N)
		{
			printf("FILE : args= ");
			for (int i = 0; i < current->arg_size; i++)
			{
				printf("[%d] '%s'", i, current->args[i]);
				if (i < current->arg_size - 1)
					printf(" ");
			}
			printf("\n");
		}
		current = current->next;
	}
	printf("\n");
}

t_token	*create_and_init_token(const char *value, t_type new_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = new_type;
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
	{
		printf("%s\n", result.error_message);
		free(result.error_message);
	}
	else
	{
		new_head = convert_data(result.tokens, current2, new_token);
		reorganize_cmd_to_start(&new_head);
		if (heredoc(new_head, lists, 0, 0) == 0)
		{
			list_clear(&new_head);
			write(1, "\n", 1);
			return ;
		}
		print_tokens_v2(new_head);
		if (!new_head)
			return ;
		
		excution(&new_head, &lists[0], &lists[1]);
	}
	free_tokens(result.tokens);
	free(input);
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
		g_glo.env = convert_to_array_v2(lists[0], g_glo.env);
		signal_setup(2);
		input = readline("APA@GOVOS> ");
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		loop_v2(input, lists);
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
