/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenmakh <rbenmakh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 21:36:26 by rbenmakh          #+#    #+#             */
/*   Updated: 2023/11/19 20:03:57 by rbenmakh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_strint(long *n)
{
	int		len;
	long	nb;

	len = 0;
	if (*n == 0)
		return (1);
	len += (*n < 0);
	if (len)
		*n = *n * -1;
	nb = *n;
	while (nb)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int nb)
{
	int		len;
	char	*ptr;
	long	n;

	n = nb;
	len = ft_strint(&n);
	ptr = (char *)malloc((len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[len--] = 0;
	*ptr = (nb < 0) * '-' + (!n) * '0';
	while (n)
	{
		ptr[len] = n % 10 + '0';
		len--;
		n /= 10;
	}
	return (ptr);
}
