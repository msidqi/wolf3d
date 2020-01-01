/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_closest_wall_helpers.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:12:08 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:12:10 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static double	ft_correct_distance(t_vec3 ray_dir, t_vec3 dir_forw,
											t_vec3 origin, t_vec3 inter_point)
{
	double cos_alpha;

	cos_alpha = ft_vec3_dot_product(dir_forw, ray_dir);
	return (ft_vec3_mag(ft_vec3_sub(inter_point, origin)) * cos_alpha);
}

void			ft_store_h(t_inter *hor_ver, t_vec3 *i_j_f, t_ray *ray,
t_tile **t)
{
	*hor_ver = (t_inter){i_j_f[0],
		ft_correct_distance(ray->dir, i_j_f[2], ray->origin, i_j_f[0]), true,
		((int)(i_j_f[0].y / TILE_HEIGHT) == (*t)->index.y) ? NORTH : SOUTH};
	*t = NULL;
}

void			ft_store_v(t_inter *hor_ver, t_vec3 *i_j_f, t_ray *ray,
t_tile **t)
{
	*hor_ver = (t_inter){i_j_f[1],
		ft_correct_distance(ray->dir, i_j_f[2], ray->origin, i_j_f[1]), true,
		((int)(i_j_f[1].x / TILE_WIDTH) == (*t)->index.x) ? WEST : EAST};
	*t = NULL;
}
