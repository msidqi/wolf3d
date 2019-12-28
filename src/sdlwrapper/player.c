
#include "wolf3d.h"

void ft_draw_player(SDL_Surface *surface, int ox, int oy)
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

void	ft_player_input(t_player *player, SDL_Event event, SDL_Surface *surface)
{
	Uint16		activated;
	Uint32		scan_code;
	pthread_t	thread_id;

	scan_code = 0;
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		scan_code = scan_code_down(event);
		activated = 1;
	}
	else if (event.type == SDL_KEYUP)
	{
		scan_code = scan_code_up(event);
		activated = 0;
	}
	switch (scan_code)
	{
		case SDL_SCANCODE_A:
			player->controller[PLAYER_STRAFE_LEFT] = activated; break;
		case SDL_SCANCODE_D:
			player->controller[PLAYER_STRAFE_RIGHT] = activated; break;
		case SDL_SCANCODE_W:
			player->controller[PLAYER_FORWARD] = activated; break;
		case SDL_SCANCODE_S:
			player->controller[PLAYER_BACKWARDS] = activated; break;
		case SDL_SCANCODE_LEFT:
			player->controller[PLAYER_TURN_LEFT] = activated; break;
		case SDL_SCANCODE_RIGHT:
			player->controller[PLAYER_TURN_RIGHT] = activated; break;
		case SDL_SCANCODE_UP:
			player->controller[PLAYER_FORWARD] = activated; break;
		case SDL_SCANCODE_DOWN:
			player->controller[PLAYER_BACKWARDS] = activated; break;
		case SDL_SCANCODE_SPACE:
			player->controller[PLAYER_TURN_BACK] = activated; break;
		case SDL_SCANCODE_O:
			player->controller[PLAYER_LOOK_UP] = activated; break;
		case SDL_SCANCODE_P:
			player->controller[PLAYER_LOOK_DOWN] = activated; break;
		case SDL_SCANCODE_T:
			pthread_create(&thread_id, NULL, &ft_save_ppm_pixels, (void *)surface);
	}
}

void ft_init_player_controller(t_player *player)
{
	size_t i;
	size_t size;
	
	size  = sizeof player->controller / sizeof player->controller[0];
	i = 0;
	while (i < size)
	{
		player->controller[i] = 0;
		i++;
	}
}

void	ft_create_player(t_player *player, Uint32 x, Uint32 y, t_vec3 look_dir)
{
	player->pos = (t_vec3){ x, y, 0 };
	player->forw = ft_vec3_normalize(look_dir);
	player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
	player->focal_len = 1;
	player->cam_hor = 1;
	player->cam_ver = 1;
	ft_init_player_controller(player);
	player->rotate = &ft_player_rotate;
	player->update_velocity = &ft_player_velocity;
	player->move = &ft_player_move;
	player->collision = &ft_check_player_collision;
	player->rotation_angle = ROTATION_ANGLE;
	player->speed = PLAYER_SPEED;
	player->height = 0;
}

void ft_player_rotate(t_player *player, double rotation_angle)
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
	if (player->controller[PLAYER_LOOK_UP] && player->height > -60)
	{
		player->height -= 4;
	}
	else if (player->controller[PLAYER_LOOK_DOWN] && player->height < 60)
	{
		player->height += 4;
	}
	else if (!player->controller[PLAYER_LOOK_DOWN] && !player->controller[PLAYER_LOOK_UP])
	{
		if (player->height > 60)
			player->height = 60;
		else if (player->height < -60)
			player->height = -60;
		if (player->height > 0 )
			player->height -= 4;
		if (player->height < 0)
			player->height += 4;
	}
	if (rotate)
	{
		player->forw =
		(t_vec3){cos(-rotation_angle) * player->forw.x - sin(-rotation_angle) * player->forw.y,
			sin(-rotation_angle) * player->forw.x + cos(-rotation_angle) * player->forw.y, 0};
		player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
	}
}

static void	ft_limit_velocity(t_vec3 future_pos, t_player *player, t_map *map)
{
	t_ray ray;
	t_vec3 direction;

	direction = ft_vec3_normalize(ft_vec3_sub(future_pos, player->pos));
	// printf("direction %f, %f\n", direction.x, direction.y);
	ft_ray_cast(&ray, player->pos, direction, map);
	ft_find_closest_wall(&ray, map, direction);
	// printf("---------> distance_to_wall %f\n", fabs(ray.ray_hit.distance_from_origin));
	if (ray.ray_hit.type == WALL && fabs(ray.ray_hit.distance_from_origin) <= (ft_vec3_mag(player->velocity)) + 0.3)
	{
		if ((ray.ray_hit.facing == NORTH || ray.ray_hit.facing == SOUTH))
			player->velocity.y = 0.0;
		else if ((ray.ray_hit.facing == EAST || ray.ray_hit.facing == WEST))
			player->velocity.x = 0.0;
	}
}

void	ft_check_player_collision(t_player *player, t_map *map)
{
	t_vec3 future_pos;
	
	// printf("%f %f\n", player->velocity.x, player->velocity.y);
	future_pos = ft_vec3_add(player->pos, player->velocity); // any direction
	ft_limit_velocity(future_pos, player, map);
	future_pos = ft_vec3_add(ft_vec3_add(player->pos, player->velocity), ft_vec3_rotate_z(player->velocity, 0.174533));//ft_vec3_scalar(player->right, 0.5));
	ft_limit_velocity(future_pos, player, map);
	future_pos = ft_vec3_add(ft_vec3_add(player->pos, player->velocity), ft_vec3_rotate_z(player->velocity, 0.174533));//ft_vec3_scalar(player->right, -0.5));
	ft_limit_velocity(future_pos, player, map);
}


void  ft_player_move(t_player *player)
{
	// player->velocity.x = 0;
	// player->velocity.x = 1;
	// printf("%f %f\n", player->velocity.x, player->velocity.y);
	player->pos = ft_vec3_add(player->pos, player->velocity);
	(void)player;
}

void  ft_player_velocity(t_player *player, double speed)
{
	// int moved;

	// moved = 0;
	speed += ft_shift_is_down() * 0.2;
	player->velocity.x = 0;
	player->velocity.y = 0;
	if (player->controller[PLAYER_STRAFE_RIGHT])
	{
		player->velocity = ft_vec3_add(ft_vec3_scalar(player->right, (speed / 2)), player->velocity);
		// moved = 1;
	}
	if (player->controller[PLAYER_STRAFE_LEFT])
	{
		player->velocity = ft_vec3_add(ft_vec3_scalar(player->right, -(speed / 2)), player->velocity);
	// printf("velocity %f, %f\n", player->velocity.x, player->velocity.y);
		// moved = 1;
	}
	if (player->controller[PLAYER_FORWARD])
	{
		player->velocity = ft_vec3_add(ft_vec3_scalar(player->forw, speed), player->velocity);
	// printf("velocity %f, %f\n", player->velocity.x, player->velocity.y);
		// moved = 1;
	}
	if (player->controller[PLAYER_BACKWARDS])
	{
		player->velocity = ft_vec3_add(ft_vec3_scalar(player->forw, -(speed)), player->velocity);
		// moved = 1;
	}
	// return (moved);
	// printf("x: %f y: %f z: %f\n", player->pos.x, player->pos.y, player->pos.z);
}

void	ft_player_physics(t_player *player, t_map *map)
{
	player->update_velocity(player, player->speed);
	player->collision(player, map);
	player->move(player);
	player->rotate(player, player->rotation_angle);
	(void)map;
}