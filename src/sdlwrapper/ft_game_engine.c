/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_game_engine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:13:54 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:13:55 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	ft_fill_background(SDL_Surface *bmp, t_player *player)
{
	double	dot;
	t_vec3	cross;
	int		xy[2];

	xy[0] = -1;
	while (++xy[0] < bmp->w)
	{
		dot = ft_vec3_dot_product(player->forw, RIGHT);
		cross = ft_vec3_cross_product(player->forw, RIGHT);
		if (ft_vec3_dot_product(FORW, cross) <= 0)
			dot = -dot;
		xy[1] = bmp->h / 2 + player->height + 1;
		while (--xy[1] >= 0)
			put_pixel32(bmp, (xy[0] + (int)(((dot + 1) / 2) * bmp->w)) %
			bmp->w, xy[1], get_sky_texture(xy[0], xy[1] - player->height, bmp));
	}
	xy[0] = -1;
	while (++xy[0] < bmp->w)
	{
		xy[1] = bmp->h / 2 + player->height - 1;
		while (++xy[1] < bmp->h)
			put_pixel32(bmp, xy[0], xy[1], 0xFF717171);
	}
}

static void	ft_fps_counter(t_sdl_data *sdl_data)
{
	static int prev_tick = 0;
	static int fps = 0;

	fps++;
	if (!prev_tick)
		prev_tick = SDL_GetTicks();
	if (((SDL_GetTicks() - prev_tick)) > 1000)
	{
		sdl_data->fps = fps;
		prev_tick = SDL_GetTicks();
		fps = 0;
	}
}

void		ft_apply_physics(t_player *player, t_map *map)
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

void		ft_apply_render(t_sdl_data *sdl_data, t_map *map,
t_player *player, t_text_layer *tl)
{
	ft_clear_screen(sdl_data);
	ft_fill_background(sdl_data->bmp, player);
	ft_draw_mini_map(sdl_data->mini_map_bmp, map, player);
	ft_handle_threads(player, map, sdl_data);
	ft_fps_counter(sdl_data);
	ft_update_screen(tl, sdl_data);
}
