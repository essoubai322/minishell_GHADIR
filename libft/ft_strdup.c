/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:31:57 by asebaai          #+#    #+#             */
/*   Updated: 2023/11/21 11:31:36 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*ptr;
	size_t	i;

	len = 0;
	i = 0;
	while (s[len])
		len++;
	ptr = (char *)ft_calloc(len + 1, sizeof(char));
	if (!ptr)
		return (0);
	while (i < len)
	{
		ptr[i] = s[i];
		i++;
	}
	return (ptr);
}
