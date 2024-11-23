/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:19:35 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/22 20:08:16 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *check_case_v1(char **input, int *i, int *k, char *result)
{
    result = calloc(strlen(*input) + 1, sizeof(char));

    while (isspace((*input)[*i]))
        (*i)++;
    while ((*input)[*i] && !isspace((*input)[*i]))
    {
        if ((*input)[*i] == '\'' || (*input)[*i] == '"')
        {
            char quote = (*input)[*i];
            result[(*k)++] = '`';
            (*i)++;
            while ((*input)[*i] && (*input)[*i] != quote)
                result[(*k)++] = (*input)[(*i)++];
            if ((*input)[*i])
                result[(*k)++] = '`';
            else
                return (free(result),NULL);
            (*i)++;
        }
        else
            while((*input)[*i] && !isspace((*input)[*i]) && (*input)[*i] != '\'' && (*input)[*i] != '"')
                result[(*k)++] = (*input)[(*i)++];
    }
    return (result);
}

int check_case(char *input, int i)
{
    char *result = NULL;
    int k = 0;
    
    result = check_case_v1(&input, &i, &k, result);
    if (ft_strchr(result,'`'))
    {
        if (counter_closes(result) == 2)
        {
            if (result[0] != '`' || result[k - 1] != '`')
                return (free(result),1);
        }
        else if (counter_closes(result) && counter_closes(result) % 2 == 0)
            return (free(result),1);
    }
    free(result);
    return(0);
}

int expand_variable(const char *input,  char *result, int *arr)
{
    int c = 0;
    char *var_name = calloc(strlen(input) + 1, sizeof(char));
    char *env_value = NULL;
    
    (arr[0])++;
    if (ft_strncmp(input + arr[0] , "?", 1) == 0)
    {
        arr[0]++;
        env_value = ft_itoa(g_status);
    }
    else
    {
        while (input[arr[0]] && (isalnum(input[arr[0]]) || input[arr[0]] == '_'))
            var_name[c++] = input[(arr[0])++];
        var_name[c] = '\0';
        if (c == 0)
        {
            env_value = ft_calloc(2, sizeof(char));
            env_value[0] = input[arr[0] - 1];
        }
        else
            env_value = ft_get_env(var_name, global.env);
    }
    if (env_value)
    {
        char **expanded_tokens = ft_split(env_value, ' ');
        int s = 0;
        while (expanded_tokens[s])
        {
            strcat(result, expanded_tokens[s]);
            if (expanded_tokens[s + 1] && !arr[2])
                strcat(result, " ");
            else if (expanded_tokens[s + 1] && arr[2])
                strcat(result, "`");
            s++;
        }
        ft_free2(&expanded_tokens);
    }
    free(env_value);
    free(var_name);
    arr[1] = strlen(result);
    return c;
}

void parse_quoted(const char *input, int *i, char *result, int *k, char quote)
{
    int arr[3];
    
    (*i)++;
    arr[0] = *i;
    arr[1] = *k;
    arr[2] = 0;
    while (input[arr[0]] && input[arr[0]] != quote)
    {
        if (quote == '"' && input[arr[0]] == '$')
            expand_variable(input,result, arr);
        else
            result[(arr[1])++] = input[(arr[0])++];
        *i = arr[0];
        *k = arr[1];
    }
    if (input[arr[0]]) (arr[0])++;
    *i = arr[0];
    *k = arr[1];
}

void parse_unquoted(const char *input, int *i, char *result, int *k)
{
    int arr[3];
    arr[0] = *i;
    arr[1] = *k;
    arr[2] = 1;
    while (input[arr[0]] && !isspace(input[arr[0]]) && input[arr[0]] != '\'' && input[arr[0]] != '"' &&
           input[arr[0]] != '|' && input[arr[0]] != '>' && input[arr[0]] != '<')
    {
        if (input[*i] == '$')
            expand_variable(input,result, arr);
        else
            result[arr[1]++] = input[arr[0]++];
        *i = arr[0];
        *k = arr[1];
    }
    *i = arr[0];
    *k = arr[1];
}
