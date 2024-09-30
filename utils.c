/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 20:26:34 by asebaai           #+#    #+#             */
/*   Updated: 2024/09/30 20:27:32 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
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

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
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
