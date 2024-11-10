/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:08:22 by rbenmakh          #+#    #+#             */
/*   Updated: 2023/11/19 20:06:26 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_pos(const char *big, const char *little, size_t len, size_t i)
{
	size_t	j;
	size_t	lenlittle;

	j = 0;
	lenlittle = ft_strlen(little);
	if (big[i] == little[j])
	{
		while (little[j] && i < len)
		{
			if (big[i] != little[j])
			{
				return (0);
			}
			j++;
			i++;
		}
		if (j == lenlittle)
			return (1);
	}
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	i = 0;
	if (little[0] == '\0')
		return ((char *)big);
	if (!len)
	{
		return (NULL);
	}
	while (i < len && big[i])
	{
		if (ft_pos(big, little, len, i))
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}
