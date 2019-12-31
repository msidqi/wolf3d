/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_resources.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 09:09:10 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 09:09:33 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void ft_clear_screen(t_sdl_data *sdl_data)
{
	SDL_FillRect(sdl_data->mini_map_bmp, NULL, 0x000000);
	SDL_FillRect(sdl_data->bmp, NULL, 0x000000);
	SDL_FillRect(sdl_data->display, NULL, 0x000000);
}

void	ft_update_screen(t_sdl_data *sdl_data)
{
	SDL_BlitSurface(sdl_data->bmp, NULL, sdl_data->display, NULL);
	SDL_BlitSurface(sdl_data->mini_map_bmp, NULL, sdl_data->display, NULL);
	SDL_UpdateWindowSurface(sdl_data->win);
}

void	ft_free_textures(void)
{
	int k = -1;
	SDL_Surface **array_to_free;
	
	array_to_free = get_all_textures();
	if (array_to_free)
		while (++k < TEXTURE_NUM)
			if (array_to_free[k])
				SDL_FreeSurface(array_to_free[k]);
}

void	ft_free_surface(t_sdl_data *sdl_data)
{
	if (sdl_data->bmp != NULL)
		SDL_FreeSurface(sdl_data->bmp);
	if (sdl_data->mini_map_bmp != NULL)
		SDL_FreeSurface(sdl_data->mini_map_bmp);
	if (sdl_data->menu[0] != NULL)
		SDL_FreeSurface(sdl_data->menu[0]);
	if (sdl_data->menu[1] != NULL)
		SDL_FreeSurface(sdl_data->menu[1]);
	if (sdl_data->win != NULL)
		SDL_DestroyWindow(sdl_data->win);
}

void	ft_graceful_shutdown(t_sdl_data *sdl_data, t_map *map, Mix_Music *backgroundsound)
{
	ft_destroy_map(map);
	ft_free_textures();
	ft_free_surface(sdl_data);
	Mix_FreeMusic(backgroundsound);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	exit(1);
}