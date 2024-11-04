#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>


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
	char *out;
    struct Token *next;
} t_token;

typedef struct LexerResult {
    t_token *tokens;
    char *error_message;
} t_lexer;

int	ft_strlen(char *str)
{
	int i = 0;
	while (str && str[i])
		i++;
	return(i);
}
char *ft_strcat(char *dest, char *src)
{
	int i = 0;
	int j = 0;
	char *result = calloc(ft_strlen(dest) + ft_strlen(src) + 1, sizeof(char));
	while (dest && dest[i])
	{
		result[i] = dest[i];
		i++;
	}
	while (src && src[j])
	{
		result[i] = src[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}

void ft_free2(char ***dest)
{
    if (!dest || !*dest)
        return;

    size_t i = 0;
    while ((*dest)[i])
    {
        free((*dest)[i]);
        (*dest)[i] = NULL;
        i++;
    }
    free(*dest);
    *dest = NULL;
}


t_token *create_token(enum TokenType type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    new_token->type = type;
    new_token->value = strdup(value);
	new_token->out = NULL;
    new_token->next = NULL;
    return (new_token);
}

static size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	index;

	index = 0;
	if (size != 0)
	{
		while (*(src + index) && --size)
			*(dest++) = *(src + index++);
		*dest = '\0';
	}
	while (*(src + index))
		index++;
	return (index);
}

static char	*malloc_1(size_t size, char const *index)
{
	char	*dest;

	dest = malloc(size);
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, index, size);
	return (dest);
}

static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	lenght_str;
	char	*str;

	if (!s)
		return (NULL);
	str = (char *)s;
	lenght_str = ft_strlen(str);
	if (start >= lenght_str && *str != '\0')
		return (strdup(""));
	if (lenght_str - start - 1 >= len)
		return (dest = malloc_1(len + 1, str + start));
	else
		return (dest = malloc_1(lenght_str - start + 1, str + start));
}
static void	*ft_free(char **dest)
{
	size_t	i;

	i = 0;
	while (dest[i])
	{
		free(dest[i]);
		dest[i] = NULL;
		i++;
	}
	free(dest);
	dest = NULL;
	return (NULL);
}

static int	count_words(const char *s, char c)
{
	int	nword;
	int	i;

	i = 0;
	nword = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			while (s[i] != '\0' && s[i] != c)
				i++;
			nword++;
		}
		if (s[i])
			i++;
	}
	return (nword);
}

static void	*put_words(char **dest, const char *s1, char c, size_t nword)
{
	size_t	i;
	size_t	j;
	size_t	the_word;
	size_t	start;

	i = 0;
	j = 0;
	the_word = 0;
	while (i < nword)
	{
		while (s1[j] == c && s1[j])
			j++;
		start = j;
		while (s1[j] != c && s1[j])
		{
			the_word++;
			j++;
		}
		dest[i] = ft_substr(s1, start, the_word);
		if (dest[i] == NULL)
			return (ft_free(dest));
		i++;
		the_word = 0;
	}
	return (0);
}

char	**ft_split(char const *s1, char c)
{
	size_t	nword;
	char	**dest;

	if (!s1 || *s1 == '\0')
	{
		dest = calloc(1, 8);
		return (dest);
	}
	nword = count_words(s1, c);
	if (nword == 0 && s1[0] == c)
	{
		dest = calloc(1, 8);
		return (dest);
	}
	if (nword == 0 || c == 0)
		nword = 1;
	dest = calloc(nword + 1, sizeof(char *));
	if (dest)
		put_words(dest, s1, c, nword);
	return (dest);
}
void add_token(t_token **head, enum TokenType type, char *value)
{
    char *stripped_value = calloc(1200500, sizeof(char));
    char **arg_space = NULL;
    
    if (type == QUOTE || type == DQUOTE)
    {
        int len = ft_strlen(value);
        if (len > 2)
        {
            strncpy(stripped_value, value + 1, len - 2);
            stripped_value[len - 2] = '\0';
        }
        else
            strcpy(stripped_value, "");
    }
    else if (type == WORD)
    {
        int i = 0;
        int k = 0;
        if (value && value[0] == '$')
        {
            while (value[i] != '\0')
            {
                if (value[i] == '$')
                {
                    dprintf(2, "=%s=\n", value);
                    char *var_name = calloc(ft_strlen(value) + 1, sizeof(char));
                    i++;
                    while (value[i] && (isalnum(value[i]) || value[i] == '_'))
                        var_name[k++] = value[i++];
                    var_name[k] = '\0';
                    k = 0;

                    char *env_value = NULL;
                    if (var_name)
                        env_value = getenv(var_name);
                    dprintf(2, "env == %s\n", env_value);
                    if (env_value)
                    {
                        if (value[i] == '$')
                        {
                            char *temp = stripped_value;
                            stripped_value = ft_strcat(stripped_value, env_value);
                            free(temp);
                        }
                        else
                        {
                            char *env_value1 = ft_strcat(env_value, value + i);
                            char *temp = stripped_value;
                            stripped_value = ft_strcat(stripped_value, env_value1);
                            free(temp);
                            free(env_value1);
                        }
                    }
                    free(var_name);
                    
                    if (value[i] != '$')
                    {
                        arg_space = ft_split(stripped_value, ' ');
                        int s = 0;
                        while (arg_space[s])
                            s++;
                        if (s > 1)
                        {
                            s = 0;
                            while (arg_space[s])
                            {
                                t_token *new_token = create_token(type, arg_space[s]);
                                if (*head == NULL)
                                    *head = new_token;
                                else
                                {
                                    t_token *current = *head;
                                    while (current->next != NULL)
                                        current = current->next;
                                    current->next = new_token;
                                }
                                s++;
                            }
                            ft_free2(&arg_space);
                            free(stripped_value);
                            return;
                        }
                        ft_free2(&arg_space);
                    }
                }
                else
                {
                    char *var_name = calloc(ft_strlen(value), sizeof(char));
                    while (value[i] && value[i] != '$')
                        var_name[k++] = value[i++];
                    var_name[k] = '\0';
                    k = 0;
                    if (var_name)
                    {
                        char *temp = stripped_value;
                        stripped_value = ft_strcat(stripped_value, var_name);
                        free(temp);
                    }
                    free(var_name);
                }
            }
        }
        else if (strchr(value, '$') != 0)
        {
            char *name = calloc(ft_strlen(value), sizeof(char));
            while (value[i] && value[i] != '$')
                name[k++] = value[i++];
            name[k] = '\0';
            k = 0;
            char *temp = stripped_value;
            stripped_value = ft_strcat(stripped_value, name);
            free(temp);
            free(name);
            
            while (value[i] != '\0')
            {
                if (value[i] == '$')
                {
                    i++;
                    dprintf(2, "APA\n");
                    char *var_name = calloc(ft_strlen(value), sizeof(char));
                    while (value[i] && (isalnum(value[i]) || value[i] == '_'))
                        var_name[k++] = value[i++];
                    var_name[k] = '\0';
                    k = 0;
                    
                    char *env_value = getenv(var_name);
					if (env_value)
                    {
                        if (value[i] == '$')
                        {
                            char *temp = stripped_value;
                            stripped_value = ft_strcat(stripped_value, env_value);
                            free(temp);
                        }
                        else
                        {
                            char *env_value1 = ft_strcat(env_value, value + i);
                            char *temp = stripped_value;
                            stripped_value = ft_strcat(stripped_value, env_value1);
                            free(temp);
                            free(env_value1);
                        }
                    }
                    free(var_name);
                    
                    if (value[i] != '$')
                    {
                        arg_space = ft_split(stripped_value, ' ');
                        int s = 0;
                        while (arg_space[s])
                            s++;
                        if (s > 1)
                        {
                            s = 0;
                            while (arg_space[s])
                            {
                                t_token *new_token = create_token(type, arg_space[s]);
                                if (*head == NULL)
                                    *head = new_token;
                                else
                                {
                                    t_token *current = *head;
                                    while (current->next != NULL)
                                        current = current->next;
                                    current->next = new_token;
                                }
                                s++;
                            }
                            ft_free2(&arg_space);
                            free(stripped_value);
                            return;
                        }
                        ft_free2(&arg_space);
                    }
                }
                else
                {
                    char *var_name = calloc(ft_strlen(value), sizeof(char));
                    while (value[i] && value[i] != '$')
                        var_name[k++] = value[i++];
                    var_name[k] = '\0';
                    k = 0;
                    if (var_name)
                    {
                        char *temp = stripped_value;
                        stripped_value = ft_strcat(stripped_value, var_name);
                        free(temp);
                    }
                    free(var_name);
                }
            }
        }
        else
            strcpy(stripped_value, value);
    }
    else
        strcpy(stripped_value, value);

    t_token *new_token = create_token(type, stripped_value);
    free(stripped_value);

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

int main()
{
    char *input = strdup("$aa$aa.a");
    t_token *result = NULL;
	add_token(&result, WORD, input); 
	t_token *current = result;
	while (current != NULL) {
		printf("Token: %s\n", current->value);
		current = current->next;
	}
	current = result;
	t_token *next;
	while (current != NULL) {
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
    free(input);
    return (0);
}