/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:18:51 by asebaai           #+#    #+#             */
/*   Updated: 2024/11/17 17:19:18 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    add_token_v0(char *value, int *i, char **stripped_value, int *k)
{
    if (value[0] != '$')
    {
        char *name = calloc(ft_strlen(value), sizeof(char));
        while (value[*i] && value[*i] != '$')
            name[(*k)++] = value[(*i)++];
        name[*k] = '\0';
        *k = 0;
        char *temp = *stripped_value;
        *stripped_value = ft_strcat(*stripped_value, name);
        free(temp);
        free(name);
    }
}

void add_token_v1(t_token2 **head, enum TokenType type)
{
    add_token_innit_head(head, global.stripped_value, type);
    free(global.stripped_value);
}

void add_token(t_token2 **head, enum TokenType type, char *value)
{   
    global.stripped_value = get_stripped_value(type, value, global.stripped_value);
    if (type == WORD)
    {
        if (value && strchr(value, '$') != 0)
        {
            add_token_v0(value, &global.o, &global.stripped_value, &global.k);
            while (value[global.o] != '\0')
            {
                if (value[global.o] == '$')
                {
                    global.env_value = ft_get_value(&global.o, value);
                    add_token_env_value(global.env_value, &global.stripped_value, value, &global.o);
                    if (value[global.o] != '$' && ft_strchr(value + global.o, '$') == 0)
                        if (add_token_check(head, global.stripped_value, type))
                            return ;
                }
                else
                    add_token_else(value, &global.o, &global.stripped_value, &global.k);
            }
        }
        else
            strcpy(global.stripped_value, value);
    }
    add_token_v1(head, type);
}

void    add_token_v2(t_token2 **head, enum TokenType type, const char *value)
{
    t_token2 *new_token = create_token(type, value);

    if (*head == NULL) 
        *head = new_token;
    else 
    {
        t_token2 *current = *head;
        while (current->next != NULL) 
            current = current->next;
        
        current->next = new_token;
    }
}

int counter_closes(char *input)
{
    int i = 0;
    int count = 0;
    while(input && input[i])
    {
        if (input[i] == '`')
            count++;
        i++;
    }
    return (count);
}
