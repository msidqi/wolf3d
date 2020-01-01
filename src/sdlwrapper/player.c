/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 15:38:27 by msidqi            #+#    #+#             */
/*   Updated: 2019/12/31 16:29:45 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_draw_player(SDL_Surface *surface, int ox, int oy)
{
	int r;
	int x;
	int y;
	int h;

	r = 4;
	x = -r;
	while (x < r)
	{
		h = sqrt(r * r - x * x);
		y = -h;
		while (y < h)
		{
			put_pixel32(surface, x + ox, y + oy, 0x99FF0000);
			y++;
		}
		x++;
	}
}

void	ft_create_player(t_player *p, double x, double y, t_vec3 look_dir)
{
	p->pos = (t_vec3){ x, y, 0 };
	p->forw = ft_vec3_normalize(look_dir);
	p->right = ft_vec3_normalize(ft_vec3_cross_product(p->forw, FORW));
	p->focal_len = 1;
	p->cam_hor = 1;
	p->cam_ver = 1;
	ft_init_player_controller(p);
	p->rotate = &ft_player_rotate;
	p->look = &ft_player_look;
	p->update_velocity = &ft_player_velocity;
	p->move = &ft_player_move;
	p->collision = &ft_check_player_collision;
	p->rotation_angle = ROTATION_ANGLE;
	p->speed = PLAYER_SPEED;
	p->height = 0;
}

void	ft_limit_player_velocity(t_vec3 future_pos, t_player *player,
t_map *map)
{
	t_ray	ray;
	t_vec3	direction;

	direction = ft_vec3_normalize(ft_vec3_sub(future_pos, player->pos));
	ft_ray_cast(&ray, player->pos, direction, map);
	ft_find_closest_wall(&ray, map, direction);
	if (ray.ray_hit.type == WALL
		&& fabs(ray.ray_hit.distance_from_origin)
			<= (ft_vec3_mag(player->velocity)) + 0.3)
	{
		if ((ray.ray_hit.facing == NORTH || ray.ray_hit.facing == SOUTH))
			player->velocity.y = 0.0;
		else if ((ray.ray_hit.facing == EAST || ray.ray_hit.facing == WEST))
			player->velocity.x = 0.0;
	}
}

void	ft_player_physics(t_player *player, t_map *map)
{
	player->update_velocity(player, player->speed);
	player->collision(player, map);
	player->move(player);
	player->rotate(player, player->rotation_angle);
	player->look(player);
}
