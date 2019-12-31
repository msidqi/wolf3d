/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_closest_wall.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 09:13:08 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 09:13:45 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_tile *get_map_tile_ver(t_index idx, t_map *m)
{
	if (idx.x >= 0 && idx.y >= 0 && idx.y < m->height && idx.x < m->width)
	{
		if (m->tiles[idx.y][idx.x].depth)
			return (&m->tiles[idx.y][idx.x]);
		if (((idx.x - 1) >= 0 && (idx.x - 1) < m->width))
			return (&m->tiles[idx.y][idx.x - 1]);
	}
	return (NULL);
}

static t_tile *get_map_tile_hor(t_index idx, t_map *m)
{
	if (idx.x >= 0 && idx.y >= 0 && idx.y < m->height && idx.x < m->width)
	{
		if (m->tiles[idx.y][idx.x].depth)
			return (&m->tiles[idx.y][idx.x]);
		if (((idx.y - 1) >= 0 && (idx.y - 1) < m->height))
			return (&m->tiles[idx.y - 1][idx.x]);
	}
	return (NULL);
}

void ft_pick_closest_intersection(t_ray *ray, t_inter hor, t_inter ver)
{
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


double	ft_correct_distance(t_vec3 ray_dir, t_vec3 dir_forw, t_vec3 origin, t_vec3 inter_point)
{
	double cos_alpha;

	cos_alpha = ft_vec3_dot_product(dir_forw, ray_dir);
	return (ft_vec3_mag(ft_vec3_sub(inter_point, origin)) * cos_alpha);
}

void ft_find_closest_wall(t_ray *ray, t_map *map, t_vec3 forward)
{
	t_tile *t[2];
	t_vec3 i;
	t_vec3 j;
	t_inter	hor;
	t_inter	ver;

	t[0] = &map->tiles[0][0];
	t[1] = &map->tiles[0][0];
	i = ray->first_hor_point;
	j = ray->first_ver_point;
	hor = (t_inter){(t_vec3){0, 0, 0}, MEGA, false, SKYBOX};
	ver = (t_inter){(t_vec3){0, 0, 0}, MEGA, false, SKYBOX};
	while ((t[0] || t[1]) && (!hor.intersected || !ver.intersected))
	{
		if (!hor.intersected && (t[0] = get_map_tile_hor(get_map_index(i, map), map)))
		{
			if (t[0]->depth == 1)
			{
				hor = (t_inter){i,
					ft_correct_distance(ray->dir, forward, ray->origin, i), true,
					((int)(i.y / TILE_HEIGHT) == t[0]->index.y) ? NORTH : SOUTH};
				t[0] = NULL;
			}
			i = ft_vec3_add(i, ray->increments_h);
		}
		if (!ver.intersected && (t[1] = get_map_tile_ver(get_map_index(j, map), map)))
		{
			if (t[1]->depth == 1)
			{
				ver = (t_inter){j,
					ft_correct_distance(ray->dir, forward, ray->origin, j), true,
					((int)(j.x / TILE_WIDTH) == t[1]->index.x) ? WEST : EAST};
				t[1] = NULL;
			}
			j = ft_vec3_add(j, ray->increments_v);
		}
	}
	ft_pick_closest_intersection(ray, hor, ver);
}