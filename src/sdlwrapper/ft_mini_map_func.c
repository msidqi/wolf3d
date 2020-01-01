/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_map_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:13:47 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:13:48 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_fill_mini_map_wall(int *x_y, SDL_Surface *surface, int color)
{
	int tmp;
	int tmp1;

	tmp1 = x_y[1];
	while (tmp1 < x_y[1] + MINI_MAP_TILE_HEIGHT)
	{
		tmp = x_y[0];
		while (tmp < MINI_MAP_TILE_WIDTH + x_y[0])
		{
			put_pixel32(surface, tmp++, tmp1, color);
		}
		tmp1++;
	}
}

void	ft_draw_mini_map(SDL_Surface *bmp, t_map *map, t_player *player)
{
	int k_l[2];
	int i_j[2];

	k_l[0] = -1;
	while (++k_l[0] < map->width)
	{
		k_l[1] = -1;
		while (++k_l[1] < map->height)
		{
			i_j[0] = k_l[0] * MINI_MAP_TILE_WIDTH;
			i_j[1] = k_l[1] * MINI_MAP_TILE_HEIGHT;
			if (map->tiles[k_l[1]][k_l[0]].depth)
				ft_fill_mini_map_wall(i_j, bmp, 0x701b5311);
			else
				ft_fill_mini_map_wall(i_j, bmp, 0x60FFFFFF);
		}
	}
	ft_draw_player(bmp, player->pos.x / MINI_MAP_RATIO_WIDTH,
	player->pos.y / MINI_MAP_RATIO_HEIGHT);
}

void	ft_draw_mini_map_wall_inter(t_ray_hit wall, SDL_Surface *bmp)
{
	int x;
	int y;

	x = wall.point.x / MINI_MAP_RATIO_WIDTH;
	y = wall.point.y / MINI_MAP_RATIO_HEIGHT;
	if (x >= 0 && y >= 0 && x < bmp->w && y < bmp->h)
		put_pixel32(bmp, x, y, 0xFFFFFFFF);
}
