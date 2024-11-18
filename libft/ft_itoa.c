/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 21:36:26 by asebaai          #+#    #+#             */
/*   Updated: 2023/11/19 20:03:57 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static	int	len_num(long num)
{
	int	i;

	i = 0;
	if (num == 0 || num < 0)
		i++;
	while (num)
	{
		num /= 10;
		i++;
	}
	return (i);
}

static	void	ft_creat(char *dest, int len, long number)
{
	int	i;

	i = 0;
	if (number < 0)
	{
		number *= -1;
		dest[i] = '-';
		i++;
	}
	while (len - i > 0)
	{
		dest[len - 1] = (number % 10) + 48;
		len--;
		number /= 10;
	}
}

char	*ft_itoa(int n)
{
	long	number;
	int		len;
	char	*dest;

	number = (long)n;
	len = len_num(number);
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	if (number == 0)
	{
		dest[0] = '0';
		dest[1] = '\0';
		return (dest);
	}
	ft_creat(dest, len, number);
	dest[len] = '\0';
	return (dest);
}