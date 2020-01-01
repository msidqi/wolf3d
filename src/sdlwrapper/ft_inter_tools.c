/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_inter_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:14:24 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:14:25 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

double		ft_limit_inter(t_vec3 position, double to_add)
{
	double total;

	total = position.x + to_add;
	if (total > INT_MAX || total < -MICRO)
		return (INT_MAX);
	return (total);
}

t_vec2int	ft_limit_index_by_map(t_vec2int index, t_map *map)
{
	if ((index.x < (map->width) && index.x >= 0)
		&& (index.y < (map->height) && index.y >= 0))
		return (index);
	return ((t_vec2int){ 1, 1});
}

t_vec3		ft_limit_inter_by_map(t_vec3 position, t_map *map)
{
	if ((position.x < (map->width * TILE_WIDTH) && position.x >= 0)
		&& (position.y < (map->height * TILE_HEIGHT) && position.y >= 0))
		return (position);
	return ((t_vec3){ -2, -2, -2});
}

t_vec2int	get_index(t_vec3 position, t_map *map)
{
	if (position.x >= -MICRO && position.y >= -MICRO
		&& position.y < (map->height * TILE_HEIGHT)
		&& position.x < (map->width * TILE_WIDTH))
		return ((t_vec2int){ (int)(position.x / TILE_WIDTH),
			(int)(position.y / TILE_HEIGHT) });
		return ((t_vec2int){ -1, -1 });
}
