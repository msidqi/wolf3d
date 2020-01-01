/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_controller.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 15:38:27 by msidqi            #+#    #+#             */
/*   Updated: 2019/12/31 19:51:15 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	ft_player_move_input(t_player *player, Uint16 actv, Uint32 code)
{
	if (code == SDL_SCANCODE_A)
		player->controller[PLAYER_STRAFE_LEFT] = actv;
	else if (code == SDL_SCANCODE_D)
		player->controller[PLAYER_STRAFE_RIGHT] = actv;
	else if (code == SDL_SCANCODE_W)
		player->controller[PLAYER_FORWARD] = actv;
	else if (code == SDL_SCANCODE_S)
		player->controller[PLAYER_BACKWARDS] = actv;
	else if (code == SDL_SCANCODE_LEFT)
		player->controller[PLAYER_TURN_LEFT] = actv;
	else if (code == SDL_SCANCODE_RIGHT)
		player->controller[PLAYER_TURN_RIGHT] = actv;
	else if (code == SDL_SCANCODE_UP)
		player->controller[PLAYER_FORWARD] = actv;
	else if (code == SDL_SCANCODE_DOWN)
		player->controller[PLAYER_BACKWARDS] = actv;
	else if (code == SDL_SCANCODE_SPACE)
		player->controller[PLAYER_TURN_BACK] = actv;
	else if (code == SDL_SCANCODE_O)
		player->controller[PLAYER_LOOK_UP] = actv;
	else if (code == SDL_SCANCODE_P)
		player->controller[PLAYER_LOOK_DOWN] = actv;
}

void		ft_player_input(t_player *player, SDL_Event event, SDL_Surface *bmp)
{
	Uint16		activated;
	Uint32		scan_code;
	pthread_t	thread_id;
	SDL_Surface *surface;

	scan_code = 0;
	activated = 0;
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		scan_code = scan_code_down(event);
		activated = 1;
	}
	else if (event.type == SDL_KEYUP)
	{
		scan_code = scan_code_up(event);
		activated = 0;
	}
	ft_player_move_input(player, activated, scan_code);
	if (scan_code == SDL_SCANCODE_T && event.type == SDL_KEYUP)
	{
		surface = ft_create_surface(BMP_WIDTH, BMP_HEIGHT, BPP);
		SDL_BlitSurface(bmp, NULL, surface, NULL);
		pthread_create(&thread_id, NULL, &ft_save_ppm_pixels, (void *)surface);
	}
}

void		ft_init_player_controller(t_player *player)
{
	size_t i;
	size_t size;

	size = sizeof(player->controller) / sizeof(player->controller[0]);
	i = 0;
	while (i < size)
	{
		player->controller[i] = 0;
		i++;
	}
}
