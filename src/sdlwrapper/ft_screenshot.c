/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_screenshot.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/25 21:36:57 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/26 22:55:44 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void             ft_ppm_format(int fd)
{
    ft_putstr_fd("P3\n", fd);
    ft_putnbr_fd(BMP_WIDTH, fd);
    ft_putchar_fd(' ', fd);
    ft_putnbr_fd(BMP_HEIGHT, fd);
    ft_putchar_fd('\n', fd);
    ft_putnbr_fd(255, fd);
    ft_putchar_fd('\n', fd);
}

static void             ft_save_screenshot(SDL_Surface *surface, int *var)
{
    unsigned char   *buff;
    unsigned char   ppm;
	Uint32 color;

   
    
    var[1] = -1;
    while (++var[1] < surface->h)
    {
        var[2] = -1;
        while (++var[2] < surface->w)
        {
            var[3] = 3;
			color = getpixel(surface, var[2], var[1]);
			buff = (unsigned char *)&color;
            while (--var[3] >= 0)
            {
                ppm = buff[var[3]];
                ft_putnbr_fd(ppm, var[0]);
                ft_putchar_fd(' ', var[0]);
            }
        }
    	ft_putchar_fd('\n', var[0]);
    }
    ft_putchar_fd('\n', var[0]);
}

void                    *ft_save_ppm_pixels(void *surface)
{
    int	var[4];
    SDL_Surface *pixels;

    pixels = (SDL_Surface *)surface;
    if (!(var[0] = open("screenshot.ppm", O_RDWR | O_CREAT, 0777)))
		pthread_exit(NULL);
	ft_ppm_format(var[0]);
	ft_save_screenshot(pixels, var);
	ft_putstr_fd("Screenshot has been saved in the root of the project\n", 1);
    SDL_FreeSurface(surface);
    pthread_exit(NULL);
}
