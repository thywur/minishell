/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:56:39 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/30 14:13:04 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	get_nbsize(int n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		n *= -1;
		size++;
	}
	while (n > 0)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	int				nb_size;
	char			*str;
	unsigned int	nb;

	nb_size = get_nbsize(n);
	str = (char *)malloc(sizeof(char) * (nb_size + 1));
	if (!str)
		return (NULL);
	str[nb_size] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		nb = n * -1;
	}
	else
		nb = n;
	if (nb == 0)
		str[0] = '0';
	while (nb > 0)
	{
		str[(nb_size--) - 1] = (nb % 10) + 48;
		nb = nb / 10;
	}
	return (str);
}
