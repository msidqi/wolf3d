/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ray_cast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 08:51:44 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 08:54:32 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	ft_find_vertical_intersection(t_ray *ray, t_map *map)
{
	double dot = ft_vec3_dot_product(ray->dir, RIGHT);
	double angle = acos(dot);												// positive angle is down (clock-wise)

	double horizontal_edge;

	t_vec3 cross = ft_vec3_cross_product(ray->dir, RIGHT);
	if (ft_vec3_dot_product(FORW, cross) < 0)
		angle = -angle;
	if ((dot < 0))
		horizontal_edge =  ray->origin.x - (ray->origin.x - (TILE_WIDTH * ((int)ray->origin.x / (int)TILE_WIDTH)));		// left edge
	else
		horizontal_edge = ray->origin.x + (TILE_WIDTH - (ray->origin.x - (TILE_WIDTH * ((int)ray->origin.x / (int)TILE_WIDTH))));	// right edge
	double distance_to_horizontal_edge = horizontal_edge - ray->origin.x;
	double distance_from_horizontal_edge_to_vertical_inter = distance_to_horizontal_edge * tan(angle);
	ray->first_ver_point = ft_limit_inter_by_map((t_vec3){ray->origin.x + distance_to_horizontal_edge, ray->origin.y + distance_from_horizontal_edge_to_vertical_inter, 0}, map);
	double next_horiz_edge_to_next_vertical_inter = TILE_WIDTH * tan(angle); // steps
	ray->increments_v.x = dot > 0 ? TILE_WIDTH : -TILE_WIDTH;
	ray->increments_v.y = dot > 0 ? next_horiz_edge_to_next_vertical_inter : -next_horiz_edge_to_next_vertical_inter;
}

static void	ft_find_horizontal_intersection(t_ray *ray, t_map *map)
{
	double dot = ft_vec3_dot_product(ray->dir, DOWN); // positive y (to know which edge to pick)
	double angle = acos(dot);											// positive angle is left (clock-wise)
	double vertical_edge;
	t_vec3 cross = ft_vec3_cross_product(ray->dir, DOWN);

	if (ft_vec3_dot_product(FORW, cross) > 0)
		angle = -angle;
	if ((dot < 0))
		vertical_edge =  ray->origin.y - (ray->origin.y - (TILE_HEIGHT * ((int)ray->origin.y / (int)TILE_HEIGHT)));		// top edge
	else
		vertical_edge = ray->origin.y + (TILE_HEIGHT - (ray->origin.y - (TILE_HEIGHT * ((int)ray->origin.y / (int)TILE_HEIGHT))));	// bottom edge
	double distance_to_vertical_edge = vertical_edge - ray->origin.y;
	double distance_from_vertical_edge_to_horizontal_inter = distance_to_vertical_edge * tan(angle);
	ray->first_hor_point = ft_limit_inter_by_map((t_vec3){ ray->origin.x + distance_from_vertical_edge_to_horizontal_inter, ray->origin.y + distance_to_vertical_edge, 0 }, map);
	double next_edge_to_next_vertical_inter = TILE_HEIGHT * tan(angle); // steps
	ray->increments_h.x = dot > 0 ? next_edge_to_next_vertical_inter : -next_edge_to_next_vertical_inter; ////// next inter point is weird af
	ray->increments_h.y = dot > 0 ? TILE_HEIGHT : -TILE_HEIGHT;
}

int ft_ray_cast(t_ray *ray, t_vec3 origin, t_vec3 direction, t_map *map)
{
	ray->dir = direction;
	ray->origin = origin;
	ft_find_vertical_intersection(ray, map);
	ft_find_horizontal_intersection(ray, map);
	return (1);
}

static void	*ft_ray_cast_scene(void	*param)
{
	int				x;
	t_vec3			mapped_pos;
	t_vec3			dir;
	t_ray			ray;
	t_thread_data	*data;

	ray.ray_hit.distance_from_origin = MEGA;
	ray.ray_hit.facing = SKYBOX;
	ray.ray_hit.type = -1;
	data = (t_thread_data *)param;
	x = data->stend.x;
	while (x < data->stend.y)
	{
		mapped_pos = ft_map_pixels_to_world(x, data->player);
		dir = ft_vec3_normalize(ft_vec3_sub(mapped_pos, data->player->pos));
		ft_ray_cast(&ray, data->player->pos, dir, data->map);
		ft_find_closest_wall(&ray, data->map, data->player->forw);
		if (ray.ray_hit.type == WALL)
		{
			ft_draw_walls(x, ray.ray_hit, data->sdl_data->bmp, data->player);
			ft_draw_mini_map_wall_inter(ray.ray_hit, data->sdl_data->mini_map_bmp);
		}
		x++;
	}
	return (NULL);
}
void	ft_handle_threads(t_player *player, t_map *map, t_sdl_data *sdl_data)
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
	copies[2].stend = (t_vec2int){copies[1].stend.y, BMP_WIDTH - (BMP_WIDTH / 4)};
	pthread_create(&ids[2], NULL, &ft_ray_cast_scene, (void *)&copies[2]);
	copies[3].stend = (t_vec2int){copies[2].stend.y, BMP_WIDTH};
	pthread_create(&ids[3], NULL, &ft_ray_cast_scene, (void *)&copies[3]);
	i = -1;
	while (++i < N_THREADS)
		pthread_join(ids[i], NULL);
}
