/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:25:46 by amoubine          #+#    #+#             */
/*   Updated: 2024/11/18 17:11:20 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

g_global global;
int		g_status;

char *string_command(const char *input, int *i)
{
    char *result = calloc(10000000, sizeof(char));
    int k = 0;
    while (isspace(input[*i]))
        (*i)++;
    
    while (input[*i] && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '\'' || input[*i] == '"')
            parse_quoted(input, i, result, &k, input[*i]);
        else
            parse_unquoted(input, i, result, &k);
    }
    result[k] = '\0';
    return result;
}

void skip_whitespace(const char *input, int *i)
{
    while (isspace(input[*i]))
        (*i)++;
}

int parse_single_quote(const char *input, int *i, char *result, int *k)
{
    (*i)++;
    while (input[*i] && input[*i] != '\'')
        result[(*k)++] = input[(*i)++];
    if (input[*i])
        (*i)++;
    return 1;
}

int parse_double_quote(const char *input, int *i, char *result, int *k)
{
    (*i)++;
    while (input[*i] && input[*i] != '"')
        result[(*k)++] = input[(*i)++];
    if (input[*i])
        (*i)++;
    return 1;
}

void parse_unquoted_heredoc1(const char *input, int *i, char *result, int *k)
{
    while (input[*i] && !isspace(input[*i]) && input[*i] != '\'' &&
           input[*i] != '"' && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        result[(*k)++] = input[(*i)++];
    }
}

char *after_heredoc1(char *input, int *i)
{
    char *result = calloc(100000, 1);
    int k = 0, flag = 0;
    int after = *i;

    skip_whitespace(input, i);
    while (input[*i] && !isspace(input[*i]) && input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
    {
        if (input[*i] == '\'')
            flag = parse_single_quote(input, i, result, &k);
        else if (input[*i] == '"')
            flag = parse_double_quote(input, i, result, &k);
        else
            parse_unquoted_heredoc1(input, i, result, &k);
    }
    
    if (!flag && strlen(result) == 0)
    {
        *i = after;
        free(result);
        return NULL;
    }
    result[k] = '\0';
    return result;
}

void while_loop(char *input, t_token2 **head)
{
    if (check_case(input, global.i) && !global.current_token_length)
    {
        global.str_cmd = string_command(input, &global.i);
        global.apa = strdup(global.str_cmd);
        free(global.str_cmd);
        if (ft_strchr(global.apa, '`'))
        {
            char **arg_space = ft_split(global.apa, '`');
            int s = 0;
            while (arg_space[s])
            {
                add_token_v2(head, WORD, arg_space[s]);
                s++;
            }
            ft_free2(&arg_space);
            free(global.apa);
            return ;
        }
        add_token_v2(head, WORD, global.apa);
        free(global.apa);
    }
    else if (!global.in_quote && !global.in_dquote)
    {
        if (input[global.i] == '|' || input[global.i] == '<' || input[global.i] == '>' || 
            input[global.i] == '\'' || input[global.i] == '"' || isspace(input[global.i])
            || (global.i > 0 && (input[global.i - 1] == '\'' || input[global.i - 1] == '"') &&  input[global.i]))
        {
            if (global.current_token_length > 0) 
            {
                if (input[global.i - 1] == '"')
                    add_token(head, DQUOTE, global.current_token);
                else if (input[global.i - 1] == '\'')
                    add_token(head, QUOTE, global.current_token);
                else
                    add_token(head, WORD, global.current_token);
                global.current_token_length = 0;
                memset(global.current_token, 0, ft_strlen(input));
            }
            if (input[global.i] == '|')
            {
                add_token(head, PIPE1, "|");
                global.i++;
            }
            else if (input[global.i] == '<')
            {
                if (input[global.i + 1] == '<') 
                {
                    add_token(head, HEREDOC1, "<<");
                    global.i += 2;
                    global.apa = after_heredoc1(input, &global.i);
                    if (global.apa)
                        add_token_v2(head, WORD, global.apa);
                    free(global.apa);
                } 
                else 
                {
                    add_token(head, REDIRECT_IN, "<");
                    global.i++;
                }
            }
            else if (input[global.i] == '>')
            {
                if (input[global.i + 1] == '>') 
                {
                    add_token(head, REDIRECT_APPEND, ">>");
                    global.i += 2;
                } 
                else 
                {
                    add_token(head, REDIRECT_OUT, ">");
                    global.i++;
                }
            }
            else if (input[global.i] == '\'')
            {
                global.in_quote = 1;
                global.current_token[global.current_token_length++] = input[global.i];
                global.i++;
            }
            else if (input[global.i] == '"')
            {
                global.in_dquote = 1;
                global.current_token[global.current_token_length++] = input[global.i];
                global.i++;
            }
            else if (isspace(input[global.i]))
                global.i++;
            else if ((input[global.i - 1] == '\'' || input[global.i - 1] == '"') &&  input[global.i])
            {
                global.current_token[global.current_token_length++] = input[global.i];
                global.i++;
            }
        }
        else
        {
            global.current_token[global.current_token_length++] = input[global.i];
            global.i++;
        }
    }
    else if (global.in_quote == 1 && input[global.i] == '\'') 
    {
        global.in_quote = 0;
        global.current_token[global.current_token_length++] = input[global.i];
        global.i++;
    }
    else if (global.in_dquote == 1 && input[global.i] == '"') 
    {
        global.in_dquote = 0;
        global.current_token[global.current_token_length++] = input[global.i];
        global.i++;
    }
    else
    {
        if (input[global.i] == '$' && global.in_dquote == 1)
        {
            char *env_value = NULL;
            char *var_name = calloc(ft_strlen(input) , sizeof(char *));
            int k = 0;

            global.i++;
            if (ft_strncmp(input + global.i, "?", 1) == 0)
            {
                global.i++;
                env_value = ft_itoa(g_status);
            }
            else
            {
                while (isalnum(input[global.i]) || input[global.i] == '_')
                    var_name[k++] = input[global.i++];
                var_name[k] = '\0';
                env_value = ft_get_env(var_name, global.env);
            }
            
            if (env_value)
            {
                strcat(global.current_token, env_value);
                global.current_token_length += ft_strlen(env_value);
            }
            else if (input[global.i - 1] == '\'')
            {
                global.current_token[global.current_token_length++] = '$';
                strcat(global.current_token, var_name);
                global.current_token_length += ft_strlen(var_name);
            }
            free(env_value);
            free(var_name);
        }
        else
        {
            global.current_token[global.current_token_length++] = input[global.i];
            global.i++;
        }
    }
}

char *check_syntax_errors(t_token2 *head, char *error_message)
{
    t_token2 *current;
    int pipe1_count;

    pipe1_count = 0;
    current = head;
    if (global.in_quote || global.in_dquote) 
        error_message = strdup("bash: unclosed quote detected");
    while (current != NULL && current->next != NULL) 
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT || 
             current->type == REDIRECT_APPEND || current->type == HEREDOC1) && 
            ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
        {
            error_message = strdup("bash: syntax error near unexpected token");
            break;
        }
        current = current->next;
    }   
    current = head;
    while (current != NULL)
    {
        if (current->type == REDIRECT_APPEND || current->type == REDIRECT_IN || current->type == REDIRECT_OUT || current->type == HEREDOC1)
        {
            if (current->next == NULL || ((current->next->type != WORD) && (current->next->type != DQUOTE) && (current->next->type = QUOTE)))
            {
                error_message = strdup("bash: syntax error near unexpected token `newline'");
                break;
            }
        }
        current = current->next;
    }
    current = head;
    while (current != NULL) 
    {
        if (current->type == PIPE1) 
        {
            pipe1_count++;
            if (pipe1_count > 0 && current->next != NULL && current->next->type == PIPE1) 
            {
                error_message = strdup("bash: syntax error near unexpected token `|'");
                break;
            }
            if (current == head || current->next == NULL) 
            {
                error_message = strdup("bash: syntax error near unexpected token `|'");
                break;
            }
        } 
        else 
            pipe1_count = 0;

        current = current->next;
    }
    return error_message;
}

t_lexer tokenize(char *input)
{
    t_token2 *head = NULL;
    inialize_global();

	global.current_token = calloc(ft_strlen(input) + 1,sizeof(global.current_token));
    
    while (input[global.i] != '\0')
        while_loop(input, &head);
    if (global.current_token_length > 0) 
    {
		if (input[global.i - 1] == '"')
            add_token(&head, DQUOTE, global.current_token);
		else if (input[global.i - 1] == '\'')
            add_token(&head, DQUOTE, global.current_token);
		else
			add_token(&head, WORD, global.current_token);
    }
    global.error_message = check_syntax_errors(head, global.error_message);
    add_token(&head, END, "");
    t_lexer result = {head, global.error_message};
	free(global.current_token);
    return (result);
}


void free_tokens(t_token2 *head) 
{
    t_token2 *current = head;
    t_token2 *next;

    while (current != NULL) 
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void print_tokens_v2(t_token *head) 
{
    t_token *current = head;
    while (current != NULL) 
    {
        if (current->type == CMD) 
        {
            printf("CMD : args= ");
            for (int i = 0; i < current->arg_size; i++) 
            {
                printf("[%d] '%s'", i,current->args[i]);
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
                printf("[%d] '%s'", i,current->args[i]);
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


void print_tokens(t_token2 *head) 
{
    t_token2 *current = head;
    while (current != NULL && current->type != END) 
    {
        printf("Token type: %d, value: %s\n", current->type, current->value);
        current = current->next;
    }
    printf("\n");
}


t_type check_last_token(t_token *current)
{
    if (current == NULL)
        return (0);
    while (current->next != NULL)
        current = current->next;
    return (current->type);
}

t_token *convert_data(t_token2 *head)
{
    t_token2 *current = head;
    t_token *new_head = NULL;
    t_token *current2 = NULL;

    while (current != NULL && current->type != END) 
    {
        if (current->type == PIPE1) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = PIPE;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup("|");
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        } 
        else if (current->type == REDIRECT_IN || current->type == REDIRECT_OUT || current->type == REDIRECT_APPEND) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = RED;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup(current->value);
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        } 
        else if (current->type == HEREDOC1) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = HEREDOC;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup("<<");
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        }
        else if ((current->type == WORD || current->type == QUOTE || current->type == DQUOTE) && 
                check_last_token(new_head) == RED)
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = FILE_N;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup(current->value);
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        }
        else if (current->type == WORD || current->type == QUOTE || current->type == DQUOTE) 
        {
            if (current2 == NULL || current2->type != CMD) 
            {
                t_token *new_token = malloc(sizeof(t_token));
                new_token->type = CMD;
                new_token->arg_size = 1;
                new_token->args = malloc(sizeof(char *) * 2);
                new_token->args[0] = strdup(current->value);
                new_token->args[1] = NULL;
                new_token->next = NULL;
                if (new_head == NULL)
                {
                    new_head = new_token;
                    current2 = new_head;
                }
                else
                {
                    current2->next = new_token;
                    current2 = current2->next;
                }
            } 
            else
            {
                current2->arg_size++;
                current2->args = realloc(current2->args, (current2->arg_size + 1) * sizeof(char *));
                if (current2->args == NULL)
                    return NULL;
                current2->args[current2->arg_size - 1] = strdup(current->value);
                current2->args[current2->arg_size] = NULL;
            }
        }
        else if (current->type == END) 
        {
            t_token *new_token = malloc(sizeof(t_token));
            new_token->type = FILE_N;
            new_token->arg_size = 1;
            new_token->args = malloc(sizeof(char *) * 2);
            new_token->args[0] = strdup(current->value);
            new_token->args[1] = NULL;
            new_token->next = NULL;
            if (new_head == NULL) 
            {
                new_head = new_token;
                current2 = new_head;
            } 
            else 
            {
                current2->next = new_token;
                current2 = current2->next;
            }
        }
        current = current->next;
    }
    return new_head;
}


char	*ft_get_env(char *name, char **env)
{
    int	i;
    int	j;
    int	len;

    i = 0;
    if (!name || !env || !*env)
        return (NULL);
    
    while (env[i])
    {
        j = 0;
        while (env[i][j] && env[i][j] != '=')
            j++;
        len = j;
        if (ft_strncmp(name, env[i], len) == 0)
            return (env[i] + len + 1);
        i++;
    }
    return (NULL);
}

void free_list(t_list *list)
{
    t_list *tmp;

    while (list)
    {
        tmp = list;
        list = list->next;
        free(tmp->content);
        free(tmp);
    }
}

void loop_v2(char *input, t_list **lists)
{
    t_lexer result = tokenize(input);

    if (result.error_message)
    {
        printf("%s\n", result.error_message);
        free(result.error_message);
    } 
    else 
    {
        t_token *new_head = convert_data(result.tokens);
        // print_tokens(result.tokens);
        // print_tokens_v2(new_head);
        if (!new_head)
            return;
        excution(&new_head, &lists[0], &lists[1]);
    }
    free_tokens(result.tokens);
    free(input);
}

void loop_free(char *input, t_list **lists)
{
    rl_clear_history();
    free(input);
    free_list(lists[0]);
    free_list(lists[1]);
    free_arr(global.env);
}

char	**convert_to_array_v2(t_list *envl,char **global_env)
{
	int		i;
	char	**cenv;
	t_list	*tmp;

	i = 0;
	tmp = envl;
    free_arr(global_env);
    (void)global_env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	cenv = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cenv)
		return (0);
	i = 0;
	while (envl)
	{
		cenv[i++] = ft_strdup((char *)envl->content);
		envl = envl->next;
	}
	cenv[i] = 0;
	return (cenv);
}

int	loop(int argc, char **argv, char **env)
{
    char *input;
    (void)argc;
    (void)argv;

    t_list	*lists[2];

    set_up_env_exp(&lists[0], &lists[1], env);
    while (1) 
    {
        // print g_status
        printf("g_status = %d\n", g_status);
        global.env = convert_to_array_v2(lists[0],global.env);
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
            continue;
        }
        add_history(input);
        loop_v2(input, lists);
    }
    loop_free(input, lists);
    return (0);
}

int	main(int argc, char **argv, char **env)
{
	return (loop(argc, argv,  env));
}
