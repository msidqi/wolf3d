/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_vectors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 14:03:14 by msidqi            #+#    #+#             */
/*   Updated: 2019/12/12 14:03:15 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgl.h"

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

static void	ft_putnbr(int nb)
{
	unsigned int x;

	if (nb < 0)
	{
		ft_putchar('-');
		x = nb * -1;
	}
	else
	{
		x = nb;
	}
	if (x >= 10)
	{
		ft_putnbr(x / 10);
		ft_putchar(x % 10 + '0');
	}
	else
	{
		ft_putchar(x + '0');
	}
}

void        ft_putvec3(t_vec3 *vec)
{
    write(1, "Vector( ", 8);
    ft_putnbr(vec->x);
    write(1, ", ", 2);
    ft_putnbr(vec->y);
    write(1, ", ", 2);
    ft_putnbr(vec->z);
    write(1, " )\n", 3);
}

void        ft_putvec4(t_vec4 *vec)
{
    write(1, "Vector( ", 8);
    ft_putnbr(vec->x);
    write(1, ", ", 2);
    ft_putnbr(vec->y);
    write(1, ", ", 2);
    ft_putnbr(vec->z);
    write(1, ", ", 2);
    ft_putnbr(vec->w);
    write(1, " )\n", 3);
}
