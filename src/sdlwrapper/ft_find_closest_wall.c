/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_closest_wall.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:11:26 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:12:00 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static t_tile	*get_tile_v(t_vec2int idx, t_map *m)
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

static t_tile	*get_tile_h(t_vec2int idx, t_map *m)
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

void			ft_pick_closest_inter(t_ray *ray, t_inter hor, t_inter ver)
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

static void		ft_init_inter_data(t_tile **t, t_inter *hor_ver, t_map *map)
{
	t[0] = &map->tiles[0][0];
	t[1] = &map->tiles[0][0];
	hor_ver[0] = (t_inter){(t_vec3){0, 0, 0}, MEGA, false, SKYBOX};
	hor_ver[1] = (t_inter){(t_vec3){0, 0, 0}, MEGA, false, SKYBOX};
}

void			ft_find_closest_wall(t_ray *ray, t_map *m, t_vec3 forw)
{
	t_tile	*t[2];
	t_vec3	i_j_f[3];
	t_inter	hr_vr[2];

	i_j_f[0] = ray->first_hor_point;
	i_j_f[1] = ray->first_ver_point;
	i_j_f[2] = forw;
	ray->ray_hit.distance_from_origin = MEGA;
	ft_init_inter_data(t, hr_vr, m);
	while ((t[0] || t[1]) && (!hr_vr[0].hashit || !hr_vr[1].hashit))
	{
		if (!hr_vr[0].hashit && (t[0] = get_tile_h(get_index(i_j_f[0], m), m)))
		{
			if (t[0]->depth == 1)
				ft_store_h(&hr_vr[0], i_j_f, ray, &t[0]);
			i_j_f[0] = ft_vec3_add(i_j_f[0], ray->increments_h);
		}
		if (!hr_vr[1].hashit && (t[1] = get_tile_v(get_index(i_j_f[1], m), m)))
		{
			if (t[1]->depth == 1)
				ft_store_v(&hr_vr[1], i_j_f, ray, &t[1]);
			i_j_f[1] = ft_vec3_add(i_j_f[1], ray->increments_v);
		}
	}
	ft_pick_closest_inter(ray, hr_vr[0], hr_vr[1]);
}
