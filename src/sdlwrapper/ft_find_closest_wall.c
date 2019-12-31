/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_closest_wall.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 09:13:08 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 09:17:32 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_tile *get_map_tile_ver(t_index m_index, t_map *map)
{
	if (m_index.x >= 0 && m_index.y >= 0 && m_index.y < map->height && m_index.x < map->width)
	{
		if (map->tiles[m_index.y][m_index.x].depth)
			return (&map->tiles[m_index.y][m_index.x]);
		if (((m_index.x - 1) >= 0 && (m_index.x - 1) < map->width))//		&& map->tiles[m_index.y - 1][m_index.x].depth)
			return (&map->tiles[m_index.y][m_index.x - 1]);
	}
	return (NULL);
}

static t_tile *get_map_tile_hor(t_index m_index, t_map *map)
{
	if (m_index.x >= 0 && m_index.y >= 0 && m_index.y < map->height && m_index.x < map->width)
	{
		if (map->tiles[m_index.y][m_index.x].depth)
			return (&map->tiles[m_index.y][m_index.x]);
		if (((m_index.y - 1) >= 0 && (m_index.y - 1) < map->height))//		&& map->tiles[m_index.y - 1][m_index.x].depth)
			return (&map->tiles[m_index.y - 1][m_index.x]);
	}
	return (NULL);
}

void ft_find_closest_wall(t_ray *ray, t_map *map, t_vec3 forward)//t_player *player)
{
	t_tile *tile = &map->tiles[0][0];
	t_tile *tile2 = &map->tiles[0][0];
	t_vec3 i = ray->first_hor_point;
	t_vec3 j = ray->first_ver_point;
	double cos_alpha;
	t_inter	hor;
	t_inter	ver;

	hor.intersected = 0;
	ver.intersected = 0;
	hor.distance = MEGA;
	ver.distance = MEGA;
	ray->ray_hit.distance_from_origin = MEGA;
	while ((tile || tile2) && (!hor.intersected || !ver.intersected))
	{
		if (!hor.intersected && (tile = get_map_tile_hor(get_map_index(i, map), map)))
		{
			if (tile->depth == 1)
			{
				cos_alpha = ft_vec3_dot_product(forward, ray->dir);
				hor.distance = ft_vec3_mag(ft_vec3_sub(i, ray->origin)) * cos_alpha;
				hor.facing = ((int)(i.y / TILE_HEIGHT) == tile->index.y) ? NORTH : SOUTH;
				hor.point = i;
				hor.intersected = 1;
				tile = NULL;
			}
			i = ft_vec3_add(i, ray->increments_h);
		}
		if (!ver.intersected && (tile2 = get_map_tile_ver( get_map_index(j, map), map )))
		{
			if (tile2->depth == 1)
			{
				cos_alpha = ft_vec3_dot_product(forward, ray->dir);
				ver.distance = ft_vec3_mag(ft_vec3_sub(j, ray->origin))  * cos_alpha;
				ver.facing = ((int)(j.x / TILE_WIDTH) == tile2->index.x) ? WEST : EAST;
				ver.point = j;
				ver.intersected = 1;
				tile2 = NULL;
			}
			j = ft_vec3_add(j, ray->increments_v);
		}
	}
	ray->ray_hit.facing = SKYBOX;
	if (hor.distance < ver.distance)
	{
		ray->ray_hit.point = hor.point;
		ray->ray_hit.distance_from_origin = hor.distance;
		ray->ray_hit.facing = hor.facing;
		ray->ray_hit.type = WALL;
	}
	else if (ver.distance >= ver.distance && ver.distance != MEGA)
	{
		ray->ray_hit.point = ver.point;
		ray->ray_hit.distance_from_origin = ver.distance;
		ray->ray_hit.facing = ver.facing;
		ray->ray_hit.type = WALL;
	}
}