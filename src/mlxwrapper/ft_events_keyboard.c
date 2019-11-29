/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_events_keyboard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 13:27:08 by msidqi            #+#    #+#             */
/*   Updated: 2019/10/11 11:10:12 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgl.h"

int		mouse_move(int x, int y, t_data *data)
{
	(void)data;
	(void)x;
	(void)y;
	return (0);
}

int		no_event_mouse_move(t_data *data)
{
	(void)data;
	return (1);
}

int		key_event(int key_code, t_data *data)
{
	(void)key_code;
	(void)data;
	return (1);
}

int		key_press(int key_code, t_data *data)
{
	pthread_t	thread_id;

	if (key_code == 53)
		red_button(data);
	else if (key_code == 1)
	{
		pthread_create(&thread_id, NULL, &ft_screenshot, (void *)data);
		return (0);
	}
	else if (key_event(key_code, data))
	{
		ft_multi_thread(data, &ft_ray_trace);
		mlx_put_image_to_window(data->mlx, data->win, data->img_ptr, 0, 0);
		return (0);
	}
	return (1);
}

int		mouse_press(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	(void)button;
	(void)data;
	return (1);
}
