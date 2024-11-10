/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 19:50:55 by rbenmakh          #+#    #+#             */
/*   Updated: 2023/11/20 12:18:46 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	ft_strcpy(char *dest, const char *source, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dest[i] = source[i];
		i++;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	lens1;
	size_t	lens2;
	char	*ptr;

	if (!s1 || !s2)
	{
		return (NULL);
	}
	len = ft_strlen(s1) + ft_strlen(s2);
	ptr = malloc((len + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	ft_strcpy(ptr, s1, lens1);
	ft_strcpy(ptr + lens1, s2, lens2);
	ptr[len] = '\0';
	return (ptr);
}
