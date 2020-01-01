/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putpixel.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:13:38 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:13:39 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int		bpp;
	Uint8	*p;

	bpp = surface->format->BytesPerPixel;
	p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		p[0] = (pixel >> 16) & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = pixel & 0xff;
	}
	else
	{
		p[0] = pixel & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = (pixel >> 16) & 0xff;
	}
}

Uint32	getpixel(SDL_Surface *surface, int x, int y)
{
	int		bpp;
	Uint8	*p;

	bpp = surface->format->BytesPerPixel;
	p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
		return (p[0] << 16 | p[1] << 8 | p[2]);
	else
		return (p[0] | p[1] << 8 | p[2] << 16);
	return (0);
}

Uint32	get_pixel32(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels;

	pixels = (Uint32 *)surface->pixels;
	return (pixels[(y * surface->w) + x]);
}

void	put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *pixels;

	pixels = (Uint32 *)surface->pixels;
	if (y < surface->h && x < surface->w && y >= 0 && x >= 0)
		pixels[(y * surface->w) + x] = pixel;
}
