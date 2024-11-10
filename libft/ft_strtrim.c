/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 09:48:23 by rbenmakh          #+#    #+#             */
/*   Updated: 2023/11/19 20:06:37 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static int	is_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	end;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (s1[i] && is_set(s1[i], set))
		i++;
	end = ft_strlen(s1);
	while (end && is_set(s1[end - 1], set))
		end--;
	return (ft_substr(s1, i, end - i));
}
