/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_menu_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 08:30:17 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 10:41:26 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_init_text_layer(t_text_layer *tl, t_sdl_data sdl_data)
{
	if (TTF_Init() == -1) 
	{
		perror(TTF_GetError());
		ft_free_surface(&sdl_data);
		exit(2);
	}
	tl->font = TTF_OpenFont("font/destroy.ttf", 45);
	if(!tl->font)
	{
		perror(TTF_GetError());
		ft_free_surface(&sdl_data);
		exit(2);
	}
	tl->color[0] = (SDL_Color){255,50,0,255};
	tl->color[1] = (SDL_Color){255,144,0,255};
	tl->text_pos.h = 100;
	tl->text_pos.w = 100;
}

void	ft_init_bgmusic(Mix_Music *backgroundsound, t_sdl_data sdl_data)
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		perror(Mix_GetError());
		ft_free_surface(&sdl_data);
		exit(2);
	}
    backgroundsound = Mix_LoadMUS("bg.wav");
	if (!backgroundsound)
	{
    	perror(Mix_GetError());
		ft_free_surface(&sdl_data);
		exit(2);
	}
	Mix_PlayMusic(backgroundsound, -1);
}