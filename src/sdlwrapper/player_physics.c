/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_physics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 15:38:27 by msidqi            #+#    #+#             */
/*   Updated: 2019/12/31 15:53:37 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_player_look(t_player *p)
{
	if (p->controller[PLAYER_LOOK_UP] && p->height > -60)
	{
		p->height -= 4;
	}
	else if (p->controller[PLAYER_LOOK_DOWN] && p->height < 60)
	{
		p->height += 4;
	}
	else if (!p->controller[PLAYER_LOOK_DOWN] && !p->controller[PLAYER_LOOK_UP])
	{
		if (p->height > 60)
			p->height = 60;
		else if (p->height < -60)
			p->height = -60;
		if (p->height > 0)
			p->height -= 4;
		if (p->height < 0)
			p->height += 4;
	}
}

void	ft_player_rotate(t_player *player, double rotation_angle)
{
	char rotate;

	rotate = 0;
	if (player->controller[PLAYER_TURN_LEFT])
	{
		rotation_angle = ROTATION_ANGLE;
		rotate = 1;
	}
	if (player->controller[PLAYER_TURN_RIGHT])
	{
		rotation_angle = -ROTATION_ANGLE;
		rotate = 1;
	}
	if (rotate)
	{
		player->forw = ft_vec3_rotate_z(player->forw, rotation_angle);
		player->right = ft_vec3_normalize(
			ft_vec3_cross_product(player->forw, FORW));
	}
}

void	ft_check_player_collision(t_player *player, t_map *map)
{
	t_vec3 future_pos;

	future_pos = ft_vec3_add(player->pos, player->velocity);
	ft_limit_player_velocity(future_pos, player, map);
	future_pos = ft_vec3_add(ft_vec3_add(player->pos, player->velocity),
		ft_vec3_rotate_z(player->velocity, 0.174533));
	ft_limit_player_velocity(future_pos, player, map);
	future_pos = ft_vec3_add(ft_vec3_add(player->pos, player->velocity),
		ft_vec3_rotate_z(player->velocity, 0.174533));
	ft_limit_player_velocity(future_pos, player, map);
}

void	ft_player_move(t_player *player)
{
	player->pos = ft_vec3_add(player->pos, player->velocity);
}

void	ft_player_velocity(t_player *p, double speed)
{
	speed += ft_shift_is_down() * 0.1;
	p->velocity.x = 0;
	p->velocity.y = 0;
	if (p->controller[PLAYER_STRAFE_RIGHT])
		p->velocity = ft_vec3_add(
			ft_vec3_scalar(p->right, (speed / 2)), p->velocity);
	if (p->controller[PLAYER_STRAFE_LEFT])
		p->velocity = ft_vec3_add(
			ft_vec3_scalar(p->right, -(speed / 2)), p->velocity);
	if (p->controller[PLAYER_FORWARD])
		p->velocity = ft_vec3_add(
			ft_vec3_scalar(p->forw, speed), p->velocity);
	if (p->controller[PLAYER_BACKWARDS])
		p->velocity = ft_vec3_add(
			ft_vec3_scalar(p->forw, -(speed)), p->velocity);
}
