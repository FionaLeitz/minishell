/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_no_malloc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 11:45:09 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/29 11:45:14 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static int	recursive(unsigned int nbr, char *buff)
{
	int				size;
	int				i;
	unsigned int	nbr2;

	size = 1;
	i = 0;
	nbr2 = nbr;
	nbr = nbr / 10;
	if (nbr > 0)
		size += recursive(nbr, buff);
	while (buff[i])
		i++;
	buff[i] = (nbr2 % 10) + '0';
	return (size);
}

int	ft_itoa_no_malloc(int nbr, char *buff)
{
	int				size;
	unsigned int	nbr2;

	ft_bzero(buff, 12);
	size = 0;
	nbr2 = nbr;
	if (nbr < 0)
	{
		buff[0] = '-';
		nbr2 = -nbr;
		size++;
	}
	size += recursive(nbr2, buff);
	return (size);
}
