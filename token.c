#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_TOKEN_LENGTH 1000

enum TokenType {
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
    HEREDOC,
    QUOTE,
    DQUOTE,
    END
};

typedef struct Token {
    enum TokenType type;
    char *value;
    struct Token *next;
} t_token;

typedef struct LexerResult {
    t_token *tokens;
    char *error_message;
} t_lexer;

t_token *create_token(enum TokenType type, const char *value) 
{
    t_token *new_token = malloc(sizeof(t_token));
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return (new_token);
}

void add_token(t_token **head, enum TokenType type, const char *value) 
{
    t_token *new_token = create_token(type, value);

    if (*head == NULL) 
        *head = new_token;
    else 
    {
        t_token *current = *head;
        while (current->next != NULL) 
            current = current->next;
        
        current->next = new_token;
    }
}

t_lexer tokenize(char *input) 
{
    t_token *head = NULL;
    int i = 0;
    char current_token[MAX_TOKEN_LENGTH] = "";
    int current_token_length = 0;
    int in_quote = 0;
    int in_dquote = 0;
    char *error_message = NULL;

    while (input[i] != '\0')
    {
        if (!in_quote && !in_dquote)
        {
            if (input[i] == '|' || input[i] == '<' || input[i] == '>' || 
                input[i] == '\'' || input[i] == '"' || isspace(input[i]))
            {
                if (current_token_length > 0) 
                {
                    add_token(&head, WORD, current_token);
                    current_token_length = 0;
                    memset(current_token, 0, MAX_TOKEN_LENGTH);
                }

                if (input[i] == '|')
                {
                    add_token(&head, PIPE, "|");
                    i++;
                }
                else if (input[i] == '<')
                {
                    if (input[i + 1] == '<') 
                    {
                        add_token(&head, HEREDOC, "<<");
                        i += 2;
                    } 
                    else 
                    {
                        add_token(&head, REDIRECT_IN, "<");
                        i++;
                    }
                }
                else if (input[i] == '>')
                {
                    if (input[i + 1] == '>') 
                    {
                        add_token(&head, REDIRECT_APPEND, ">>");
                        i += 2;
                    } 
                    else 
                    {
                        add_token(&head, REDIRECT_OUT, ">");
                        i++;
                    }
                }
                else if (input[i] == '\'')
                {
                    in_quote = 1;
                    current_token[current_token_length++] = input[i];
                    i++;
                }
                else if (input[i] == '"')
                {
                    in_dquote = 1;
                    current_token[current_token_length++] = input[i];
                    i++;
                }
                else if (isspace(input[i]))
                {
                    i++;
                }
            }
            else
            {
                current_token[current_token_length++] = input[i];
                i++;
            }
        } 
        else if (in_quote && input[i] == '\'') 
        {
            in_quote = 0;
            current_token[current_token_length++] = input[i];
            i++;
        }
        else if (in_dquote && input[i] == '"') 
        {
            in_dquote = 0;
            current_token[current_token_length++] = input[i];
            i++;
        }
        else
        {
            current_token[current_token_length++] = input[i];
            i++;
        }
    }

    if (current_token_length > 0) 
    {
        add_token(&head, WORD, current_token);
    }

    // Check for unclosed quotes
    if (in_quote || in_dquote) 
    {
        error_message = strdup("bash: unclosed quote detected");
    }

    // Check for mismatched redirections
    t_token *current = head;
    while (current != NULL && current->next != NULL) 
    {
        if ((current->type == REDIRECT_IN || current->type == REDIRECT_OUT || 
             current->type == REDIRECT_APPEND || current->type == HEREDOC) && 
            (current->next->type != WORD))
        {
            error_message = strdup("bash: syntax error near unexpected token");
            break;
        }
        current = current->next;
    } 

    // Check for syntax redirection errors
    current = head;
    while (current != NULL)
    {
        if (current->type == REDIRECT_APPEND || current->type == REDIRECT_IN || current->type == REDIRECT_OUT || current->type == HEREDOC)
        {
            if (current->next == NULL || current->next->type != WORD)
            {
                error_message = strdup("bash: syntax error near unexpected token `newline'");
                break;
            }
        }
        current = current->next;
    }

    // Check for pipe errors
    current = head;
    int pipe_count = 0;
    while (current != NULL) 
    {
        if (current->type == PIPE) 
        {
            pipe_count++;
            if (pipe_count > 0 && current->next != NULL && current->next->type == PIPE) 
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
            pipe_count = 0;

        current = current->next;
    }
    add_token(&head, END, "");
    t_lexer result = {head, error_message};
    return (result);
}

void free_tokens(t_token *head) 
{
    t_token *current = head;
    t_token *next;

    while (current != NULL) 
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void print_tokens(t_token *head) 
{
    t_token *current = head;
    while (current != NULL && current->type != END) 
    {
        printf("Token type: %d, value: %s\n", current->type, current->value);
        current = current->next;
    }
    printf("\n");
}

int main() {
    char *input;

    while (1) 
    {
        // Use readline to get input
        input = readline("minishell> ");

        // Check for EOF (Ctrl+D)
        if (!input) 
        {
            printf("\nExiting minishell...\n");
            break;
        }

        // Add input to readline history
        add_history(input);

        // Tokenize the input
        t_lexer result = tokenize(input);

        if (result.error_message)
        {
            printf("%s\n", result.error_message);
            free(result.error_message);
        } 
        else 
        {
            // Print the tokens (for debugging purposes)
            print_tokens(result.tokens);
        }

        // Free the tokens and input
        free_tokens(result.tokens);
        free(input);
    }

    // Clean up readline history
    rl_clear_history();

    return 0;
}