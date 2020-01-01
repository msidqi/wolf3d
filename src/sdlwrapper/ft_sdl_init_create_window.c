/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sdl_init_create_window.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:13:14 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:13:16 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static SDL_Window	*ft_sdl_init_create_window(
	int win_pos_x, int win_pos_y, int win_width, int win_height)
{
	SDL_Window *win;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		perror(SDL_GetError());
		exit(EXIT_FAILURE);
	}
	win = SDL_CreateWindow(
		"wolf3d", win_pos_x, win_pos_y, win_width,
											win_height, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		perror(SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return (win);
}

void				ft_sdl_init_data(t_sdl_data *sdl_data)
{
	sdl_data->fps = 0;
	sdl_data->quit = false;
	sdl_data->startgame = false;
	sdl_data->bmp = NULL;
	sdl_data->mini_map_bmp = NULL;
	sdl_data->win = ft_sdl_init_create_window(500, 400, WIN_WIDTH, WIN_HEIGHT);
	sdl_data->bmp = ft_create_surface(BMP_WIDTH, BMP_HEIGHT, BPP);
	sdl_data->mini_map_bmp = ft_create_surface(BMP_WIDTH, BMP_HEIGHT, BPP);
	if (!sdl_data->bmp || !sdl_data->mini_map_bmp)
	{
		perror(SDL_GetError());
		if (sdl_data->bmp != NULL)
			SDL_FreeSurface(sdl_data->bmp);
		if (sdl_data->win != NULL)
			SDL_DestroyWindow(sdl_data->win);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	sdl_data->display = SDL_GetWindowSurface(sdl_data->win);
	sdl_data->menu[0] = SDL_LoadBMP("image.bmp");
	sdl_data->menu[1] = SDL_LoadBMP("options.bmp");
}
