/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 20:57:35 by asebaai          #+#    #+#             */
/*   Updated: 2023/11/21 11:02:45 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <limits.h> 

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (size && nmemb > SIZE_MAX / size)
		return (NULL);
	ptr = (void *)malloc(nmemb * size);
	if (!ptr)
		return (0);
	ft_bzero(ptr, size * nmemb);
	return (ptr);
}
