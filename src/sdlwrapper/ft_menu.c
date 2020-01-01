/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_menu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 08:25:18 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 17:38:18 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	ft_menu_choice_manager(int flags[2])
{
	if (flags[0] == 0 && flags[1] == 0)
	{
		flags[1] = 1;
		flags[0] = 10;
	}
	if (flags[0] == 2 && flags[1] == 0)
	{
		flags[1] = 2;
		flags[0] = 10;
	}
	if (flags[0] == 4 && flags[1] == 0)
	{
		flags[1] = 3;
		flags[0] = 10;
	}
}

static void	ft_menu_choice_events(t_sdl_data *sdl_data, int flags[2])
{
	if (sdl_data->event.key.keysym.scancode == SDL_SCANCODE_UP)
	{
		if (flags[1] == 0)
		{
			flags[0]--;
			if (flags[0] < 0 && flags[0] != -1)
				flags[0] = 0;
		}
	}
	if (sdl_data->event.key.keysym.scancode == SDL_SCANCODE_DOWN)
	{
		if (flags[1] == 0)
		{
			flags[0]++;
			if (flags[0] > 4)
				flags[0] = 4;
		}
	}
}

static void	ft_create_text_layers(t_menu_env *menu_env, t_sdl_data *sdl_data,
int flag)
{
	menu_env->tl.text_pos.x = 645;
	menu_env->tl.text_pos.y = 178;
	menu_env->tl.options[0] = TTF_RenderText_Solid(menu_env->tl.font, "START",
	menu_env->tl.color[flag & 001 ? 1 : 0]);
	menu_env->tl.options[1] = TTF_RenderText_Solid(menu_env->tl.font, "OPTIONS",
	menu_env->tl.color[flag & 010 ? 1 : 0]);
	menu_env->tl.options[2] = TTF_RenderText_Solid(menu_env->tl.font, "QUIT",
	menu_env->tl.color[flag & 100 ? 1 : 0]);
	SDL_BlitSurface(menu_env->tl.options[0], NULL, sdl_data->display,
	&menu_env->tl.text_pos);
	menu_env->tl.text_pos.y += 100;
	menu_env->tl.text_pos.x -= 25;
	SDL_BlitSurface(menu_env->tl.options[1], NULL, sdl_data->display,
	&menu_env->tl.text_pos);
	menu_env->tl.text_pos.y += 100;
	menu_env->tl.text_pos.x += 55;
	SDL_BlitSurface(menu_env->tl.options[2], NULL, sdl_data->display,
	&menu_env->tl.text_pos);
	SDL_FreeSurface(menu_env->tl.options[0]);
	SDL_FreeSurface(menu_env->tl.options[1]);
	SDL_FreeSurface(menu_env->tl.options[2]);
	SDL_UpdateWindowSurface(sdl_data->win);
}

static void	ft_menu_show(t_sdl_data *sdl_data, int flags[2],
t_menu_env *menu_env, t_map *map)
{
	SDL_BlitSurface(sdl_data->menu[0], NULL, sdl_data->display, NULL);
	if (flags[0] == 0)
		ft_create_text_layers(menu_env, sdl_data, 0 | 110);
	else if (flags[0] == 2)
		ft_create_text_layers(menu_env, sdl_data, 0 | 101);
	else if (flags[0] == 4)
		ft_create_text_layers(menu_env, sdl_data, 0 | 011);
	if (flags[1] == 1)
		sdl_data->startgame = true;
	if (flags[1] == 2)
	{
		SDL_BlitSurface(sdl_data->menu[1], NULL, sdl_data->display, NULL);
		SDL_UpdateWindowSurface(sdl_data->win);
	}
	if (flags[1] == 3)
	{
		flags[1] = -1;
		sdl_data->quit = true;
		ft_graceful_shutdown(sdl_data, map, menu_env);
	}
}

void		ft_menu_loop_content(t_sdl_data *sdl_data, int flags[2],
t_menu_env *menu_env, t_map *map)
{
	while (!sdl_data->startgame)
	{
		while (SDL_PollEvent(&sdl_data->event))
		{
			if (sdl_data->event.type == SDL_QUIT ||
			sdl_data->event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				sdl_data->quit = true;
				flags[1] = 3;
			}
			if (sdl_data->event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER ||
			scan_code_up(sdl_data->event) == 40)
				ft_menu_choice_manager(flags);
			if (sdl_data->event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
			{
				flags[1] = 0;
				flags[0] = 0;
			}
			ft_menu_choice_events(sdl_data, flags);
		}
		ft_menu_show(sdl_data, flags, menu_env, map);
	}
}
