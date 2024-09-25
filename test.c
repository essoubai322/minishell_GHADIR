#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

char *string_command(char *input, int *i)
{
    char *result = calloc(strlen(input) + 1, 8);
    int k = 0;
    
    while (isspace(input[*i]))
        *i++;
    while (input[*i] && !isspace(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            if (input[*i] == '\'')
            {
                *i++;
                while (input[*i] != '\'')
                    result[k++] = input[*i++];
                *i++;
            }
            else
            {
                *i++;
                while (input[*i] != '"')
                    result[k++] = input[*i++];
                *i++;
            }
        }
        while(input[*i] && !isspace(input[*i]) && (input[*i] != '\'' && input[*i] != '"'))
            result[k++] = input[*i++];
    }
    return (result);
}
int main() {
    char *input;

    while (1) 
    {
        int i = 0;
        input = readline("minishell> ");

        if (!input) 
        {
            pintf("\nExiting minishell...\n");
            break;
        }
        add_history(input);
        char *s = string_command(input,&i);
        pintf("%s\n", s);
        free(input);
    }
    rl_clear_history();
    return (0);
}