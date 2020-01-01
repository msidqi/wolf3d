/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray_cast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:15:00 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:15:01 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	ft_find_vertical_intersection(t_ray *ray, t_map *map)
{
	double	dot_ang_dis[6];
	t_vec3	cross;

	cross = ft_vec3_cross_product(ray->dir, RIGHT);
	dot_ang_dis[0] = ft_vec3_dot_product(ray->dir, RIGHT);
	dot_ang_dis[1] = acos(dot_ang_dis[0]);
	if (ft_vec3_dot_product(FORW, cross) < 0)
		dot_ang_dis[1] = -dot_ang_dis[1];
	if (dot_ang_dis[0] < 0)
		dot_ang_dis[2] = (int)ray->origin.x;
	else
		dot_ang_dis[2] = ray->origin.x + (TILE_WIDTH - (ray->origin.x
		- (TILE_WIDTH * ((int)ray->origin.x / TILE_WIDTH))));
	dot_ang_dis[3] = dot_ang_dis[2] - ray->origin.x;
	dot_ang_dis[4] = dot_ang_dis[3] * tan(dot_ang_dis[1]);
	ray->first_ver_point = ft_limit_inter_by_map((t_vec3){ray->origin.x
	+ dot_ang_dis[3], ray->origin.y + dot_ang_dis[4], 0}, map);
	dot_ang_dis[5] = TILE_WIDTH * tan(dot_ang_dis[1]);
	ray->increments_v.x = dot_ang_dis[0] > 0 ? TILE_WIDTH : -TILE_WIDTH;
	ray->increments_v.y = dot_ang_dis[0] > 0 ? dot_ang_dis[5] : -dot_ang_dis[5];
}

static void	ft_find_horizontal_intersection(t_ray *ray, t_map *map)
{
	double	dot_ang_dis[6];
	t_vec3	cross;

	dot_ang_dis[0] = ft_vec3_dot_product(ray->dir, DOWN);
	dot_ang_dis[1] = acos(dot_ang_dis[0]);
	cross = ft_vec3_cross_product(ray->dir, DOWN);
	if (ft_vec3_dot_product(FORW, cross) > 0)
		dot_ang_dis[1] = -dot_ang_dis[1];
	if (dot_ang_dis[0] < 0)
		dot_ang_dis[2] = (int)ray->origin.y;
	else
		dot_ang_dis[2] = ray->origin.y + (TILE_HEIGHT - (ray->origin.y
		- (TILE_HEIGHT * ((int)ray->origin.y / TILE_HEIGHT))));
	dot_ang_dis[3] = dot_ang_dis[2] - ray->origin.y;
	dot_ang_dis[4] = dot_ang_dis[3] * tan(dot_ang_dis[1]);
	ray->first_hor_point = ft_limit_inter_by_map((t_vec3){ ray->origin.x
	+ dot_ang_dis[4], ray->origin.y + dot_ang_dis[3], 0 }, map);
	dot_ang_dis[5] = TILE_HEIGHT * tan(dot_ang_dis[1]);
	ray->increments_h.x = dot_ang_dis[0] > 0 ? dot_ang_dis[5] : -dot_ang_dis[5];
	ray->increments_h.y = dot_ang_dis[0] > 0 ? TILE_HEIGHT : -TILE_HEIGHT;
}

int			ft_ray_cast(t_ray *ray, t_vec3 origin, t_vec3 direction, t_map *map)
{
	ray->dir = direction;
	ray->origin = origin;
	ft_find_vertical_intersection(ray, map);
	ft_find_horizontal_intersection(ray, map);
	return (1);
}

static void	*ft_ray_cast_scene(void *param)
{
	int				x;
	t_vec3			mp_dr[2];
	t_ray			ray;
	t_thread_data	*data;

	ray.ray_hit.distance_from_origin = MEGA;
	ray.ray_hit.facing = SKYBOX;
	ray.ray_hit.type = -1;
	data = (t_thread_data *)param;
	x = data->stend.x - 1;
	while (++x < data->stend.y)
	{
		mp_dr[0] = ft_map_pixels_to_world(x, data->player);
		mp_dr[1] = ft_vec3_normalize(ft_vec3_sub(mp_dr[0], data->player->pos));
		ft_ray_cast(&ray, data->player->pos, mp_dr[1], data->map);
		ft_find_closest_wall(&ray, data->map, data->player->forw);
		if (ray.ray_hit.type == WALL)
		{
			ft_draw_walls(x, ray.ray_hit, data->sdl_data->bmp,
			data->player);
			ft_draw_mini_map_wall_inter(ray.ray_hit,
			data->sdl_data->mini_map_bmp);
		}
	}
	return (NULL);
}

void		ft_handle_threads(t_player *player, t_map *map,
t_sdl_data *sdl_data)
{
	pthread_t		ids[N_THREADS];
	t_thread_data	thread_data;
	t_thread_data	copies[N_THREADS];
	int				i;

	thread_data.map = map;
	thread_data.player = player;
	thread_data.sdl_data = sdl_data;
	i = -1;
	while (++i < N_THREADS)
		copies[i] = thread_data;
	copies[0].stend = (t_vec2int){0, BMP_WIDTH / 4};
	pthread_create(&ids[0], NULL, &ft_ray_cast_scene, (void *)&copies[0]);
	copies[1].stend = (t_vec2int){copies[0].stend.y, BMP_WIDTH / 2};
	pthread_create(&ids[1], NULL, &ft_ray_cast_scene, (void *)&copies[1]);
	copies[2].stend = (t_vec2int){copies[1].stend.y, BMP_WIDTH -
	(BMP_WIDTH / 4)};
	pthread_create(&ids[2], NULL, &ft_ray_cast_scene, (void *)&copies[2]);
	copies[3].stend = (t_vec2int){copies[2].stend.y, BMP_WIDTH};
	pthread_create(&ids[3], NULL, &ft_ray_cast_scene, (void *)&copies[3]);
	i = -1;
	while (++i < N_THREADS)
		pthread_join(ids[i], NULL);
}
