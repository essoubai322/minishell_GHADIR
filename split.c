/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:12:03 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/27 11:14:59 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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