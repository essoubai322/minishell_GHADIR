/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:32:21 by asebaai          #+#    #+#             */
/*   Updated: 2023/11/19 20:04:34 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*pdest;
	unsigned char	*psrc;

	i = 0;
	pdest = dest;
	psrc = (unsigned char *)src;
	if (!dest && !src)
	{
		return (0);
	}
	while (i < n)
	{
		pdest[i] = psrc[i];
		i++;
	}
	return (dest);
}
