/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_game_engine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 09:02:25 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 09:06:12 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void ft_fill_background(SDL_Surface *bmp, t_player *player)
{
	int x;
	int y;

	x = 0;
	while (x < bmp->w)
	{
		double dot = ft_vec3_dot_product(player->forw, RIGHT);
		t_vec3 cross = ft_vec3_cross_product(player->forw, RIGHT);
		if (ft_vec3_dot_product(FORW, cross) <= 0)
			dot = -dot;
		y = bmp->h / 2 + player->height;
		while (y >= 0)
		{
			put_pixel32(bmp, (x + (int)(((dot + 1) / 2) * bmp->w)) % bmp->w, y, get_sky_texture(x, y - player->height, bmp));
			y--;
		}
		x++;
	}
	x = 0;
	while (x < bmp->w)
	{
		y = bmp->h / 2 + player->height;
		while (y < bmp->h)
		{
			 put_pixel32(bmp, x, y, 0xFF717171);
			 y++;
		}
		x++;
	}
}

static void ft_fps_counter(void)
{
	static int prev_tick = 0;
	static int fps = 0;

	fps++;
	if (!prev_tick)
		prev_tick = SDL_GetTicks();
	if (((SDL_GetTicks() - prev_tick)) > 1000)
	{
		printf("\033[2J\nfps %d\n", fps);
		prev_tick = SDL_GetTicks();
		fps = 0;
	}
}

void	ft_apply_physics(t_player *player, t_map *map)
{
	static Uint64 previous_tick = 0;
	
	if (!previous_tick)
		previous_tick = SDL_GetPerformanceCounter();
	if (SDL_GetPerformanceCounter() - previous_tick > (Uint64)SECOND / 60)
	{
		ft_player_physics(player, map);
		previous_tick = SDL_GetPerformanceCounter();
	}
}

void ft_apply_render(t_sdl_data *sdl_data, t_map *map, t_player *player)
{
	ft_clear_screen(sdl_data);
	ft_fps_counter();
	ft_fill_background(sdl_data->bmp, player);
	ft_draw_mini_map(sdl_data->mini_map_bmp, map, player);
	ft_handle_threads(player, map, sdl_data);
	ft_update_screen(sdl_data);
}