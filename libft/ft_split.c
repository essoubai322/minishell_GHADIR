/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:20:33 by rbenmakh          #+#    #+#             */
/*   Updated: 2024/09/16 09:38:25 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

static char	**ft_free(char **ptr, int j)
{
	while (j >= 0)
	{
		free(ptr[j]);
		j--;
	}
	free(ptr);
	return (NULL);
}

static int	ft_words(const char *str, char charset)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (str[i])
	{
		if (str[i] == charset)
		{
			i++;
			continue ;
		}
		else
		{
			while (str[i] != charset && str[i])
				i++;
		}
		words++;
	}
	return (words);
}

static void	ft_strcpy(char *str, char *dest, int start, int end)
{
	int	i;

	i = 0;
	while (start <= end)
	{
		dest[i] = str[start];
		start++;
		i++;
	}
	dest[i] = '\0';
}

static char	**ft_fill(char const *str, char charset, int words, int i)
{
	char	**ptr;
	int		j;
	int		start;

	j = 0;
	ptr = (char **)malloc((words + 1) * sizeof(char *));
	if (ptr == NULL)
		return (0);
	while (j < words && str[++i])
	{
		while (str[i] == charset)
			i++;
		if (str[i] == '\0')
			continue ;
		start = i;
		while (str[i] && str[i] != charset)
			i++;
		ptr[j] = malloc(i - start + 1);
		if (ptr[j] == NULL)
			return (ft_free(ptr, j));
		ft_strcpy((char *)str, ptr[j], start, i - 1);
		j++;
	}
	ptr[j] = 0;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;
	int		wordslen;

	if (!s || !*s)
		return (NULL);
	wordslen = ft_words(s, c);
	ptr = ft_fill(s, c, wordslen, -1);
	if (ptr == NULL)
		return (NULL);
	return (ptr);
}
