/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_texture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 08:39:24 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 08:43:04 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	initializes textures on first call only
**	returns an array of SDL surfaces whenever called
*/

SDL_Surface **get_all_textures(void)
{
	static SDL_Surface *game_textures[TEXTURE_NUM];
	static int initialized = 0;
	int i;

	if (!initialized)
	{
		initialized = 1;
		ft_putendl_fd("Loading Textures...", 1);
		game_textures[0] = IMG_Load("./Textures/NORTH.JPG");
		game_textures[1] = IMG_Load("./Textures/SOUTH.JPG");
		game_textures[2] = IMG_Load("./Textures/EAST.JPG");
		game_textures[3] = IMG_Load("./Textures/WEST.JPG");
		game_textures[4] = IMG_Load("./Textures/SKY6.JPG");
		i = -1;
		while (++i < TEXTURE_NUM)
			if (!game_textures[i])
			{
				ft_putendl_fd("Texture file missing.", 2);
				return (NULL);
			}

	}
	return (game_textures);
}

int get_wall_texture(int y, t_ray_hit wall, SDL_Surface *bmp)
{	
	SDL_Surface **textures;
	int color;
	textures = get_all_textures();
	int ny = (textures[wall.facing]->h) * (y - (bmp->h / 2 - wall.wall_height / 2)) / wall.wall_height;
	int nx;
	if (wall.facing == NORTH || wall.facing == SOUTH)
	{
		nx = (textures[wall.facing]->w) * (ceil(wall.wall_height - wall.point.x) - (wall.wall_height - wall.point.x));
		color = getpixel(textures[wall.facing], nx, (ny));
	}
	else if (wall.facing == EAST || wall.facing == WEST)
	{
		nx = (textures[wall.facing]->w) * (ceil(wall.wall_height - wall.point.y) - (wall.wall_height - wall.point.y));
		color = getpixel(textures[wall.facing], nx, (ny));
	}
	if (wall.facing == SKYBOX)
		return (0xFFFFFFFF);
	return (0xFF000000 + color);
}

int get_sky_texture(int x, int y, SDL_Surface *bmp)
{
	int color;
	SDL_Surface *sky;

	sky = get_all_textures()[4];
			// printf("mapped x: %d | mapped y: %d\n", (int)(((double)x / bmp->w) * sky->w), (int)(((double)y / bmp->w) * sky->h));
	color = getpixel(sky,
	(int)(((double)x / bmp->w) * sky->w)
	, (int)(((double)y / bmp->h + 0.3) * sky->h));
	return (0xFF000000 + color);
}