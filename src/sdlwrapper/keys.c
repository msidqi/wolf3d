/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:13:22 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:13:24 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int		key_code(SDL_Event event)
{
	return (event.key.keysym.scancode);
}

int		scan_code(SDL_Event event)
{
	return (event.key.keysym.sym);
}

int		scan_code_up(SDL_Event event)
{
	if (event.type == SDL_KEYUP)
		return (event.key.keysym.scancode);
	return (0);
}

int		scan_code_down(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
		return (event.key.keysym.scancode);
	return (0);
}

int		ft_shift_is_down(void)
{
	return ((SDL_GetModState() & KMOD_SHIFT));
}
