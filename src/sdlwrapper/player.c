
#include "wolf3d.h"

void	ft_player_controller(t_player *player, SDL_Event event)
{
	Uint32 scan_code_d = scan_code_down(event);
	Uint32 scan_code_u = scan_code_up(event);
	
	if ( event.type == SDL_KEYDOWN && event.key.repeat == 0 )
	{
		printf("player->controller[PLAYER_TURN_RIGHT] %d\n", player->controller[PLAYER_TURN_RIGHT]);
		switch (scan_code_d)
		{
			case SDL_SCANCODE_D:
				player->pos = ft_vec3_add(player->right, player->pos);
				player->controller[PLAYER_STRAFE_LEFT] = 1;
				break;
			case SDL_SCANCODE_A:
				player->pos = ft_vec3_add(ft_vec3_scalar(player->right, -1), player->pos);
				player->controller[PLAYER_STRAFE_RIGHT] = 1;
				break;
			case SDL_SCANCODE_W:
				player->controller[PLAYER_FORWARD] = 1;
				player->pos = ft_vec3_add(player->forw, player->pos);
				break;
			case SDL_SCANCODE_S:
				player->controller[PLAYER_BACKWARDS] = 1;
				player->pos = ft_vec3_add(ft_vec3_scalar(player->forw, -1), player->pos);
				break;
			case SDL_SCANCODE_LEFT:
				player->controller[PLAYER_TURN_LEFT] = 1;
				player->forw = (t_vec3){
					cos(-ROTATION_ANGLE) * player->forw.x - sin(-ROTATION_ANGLE) * player->forw.y,
					sin(-ROTATION_ANGLE) * player->forw.x + cos(-ROTATION_ANGLE) * player->forw.y
					, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_RIGHT:
				player->controller[PLAYER_TURN_RIGHT] = 1;
				player->forw = (t_vec3){
					cos(ROTATION_ANGLE) * player->forw.x - sin(ROTATION_ANGLE) * player->forw.y,
					sin(ROTATION_ANGLE) * player->forw.x + cos(ROTATION_ANGLE) * player->forw.y
					, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_UP:
				player->controller[PLAYER_FORWARD] = 1;
				player->pos = ft_vec3_add(player->forw, player->pos);
				break;
			case SDL_SCANCODE_DOWN:
				player->controller[PLAYER_BACKWARDS] = 1;
				player->pos = ft_vec3_add(ft_vec3_scalar(player->forw, -1), player->pos);
				break;
			case SDL_SCANCODE_G:
				player->forw = (t_vec3){-1, 0, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_J:
				player->forw = (t_vec3){1, 0, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_Y:
				player->forw = (t_vec3){0, -1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_H:
				player->forw = (t_vec3){0, 1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_U:
				player->forw = (t_vec3){1, -1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_T:
				player->forw = (t_vec3){-1, -1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_M:
				player->forw = (t_vec3){1, 1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_N:
				player->forw = (t_vec3){-1, 1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		printf("player->controller[PLAYER_TURN_RIGHT] %d\n", player->controller[PLAYER_TURN_RIGHT]);
		switch (scan_code_u)
		{
			case SDL_SCANCODE_D:
				player->controller[PLAYER_STRAFE_LEFT] = 0;
				break;
			case SDL_SCANCODE_A:
				player->controller[PLAYER_STRAFE_RIGHT] = 0;
				break;
			case SDL_SCANCODE_W:
				player->controller[PLAYER_FORWARD] = 0;
				break;
			case SDL_SCANCODE_S:
				player->controller[PLAYER_BACKWARDS] = 0;
				break;
			case SDL_SCANCODE_LEFT:
				player->controller[PLAYER_TURN_LEFT] = 0;
				break;
			case SDL_SCANCODE_RIGHT:
				player->controller[PLAYER_TURN_RIGHT] = 0;
				break;
			case SDL_SCANCODE_UP:
				player->controller[PLAYER_FORWARD] = 0;
				break;
			case SDL_SCANCODE_DOWN:
				player->controller[PLAYER_BACKWARDS] = 0;
				break;
		}
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
	player->move = &ft_player_move;
	player->rotation_angle = ROTATION_ANGLE;
	player->speed = PLAYER_SPEED;
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
	if (rotate)
	{
		player->forw =
		(t_vec3){cos(-rotation_angle) * player->forw.x - sin(-rotation_angle) * player->forw.y,
			sin(-rotation_angle) * player->forw.x + cos(-rotation_angle) * player->forw.y, 0};
		player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
	}
}

int  ft_player_move(t_player *player, double speed)
{
	int moved;

	moved = 0;
	if (player->controller[PLAYER_STRAFE_RIGHT])
	{
		player->to_move = ft_vec3_scalar(player->right, (speed / 2));
		moved = 1;
	}
	if (player->controller[PLAYER_STRAFE_LEFT])
	{
		player->to_move = ft_vec3_scalar(player->right, -(speed / 2));
		moved = 1;
	}
	if (player->controller[PLAYER_FORWARD])
	{
		player->to_move = ft_vec3_scalar(player->forw, speed);
		moved = 1;
	}
	if (player->controller[PLAYER_BACKWARDS])
	{
		player->to_move = ft_vec3_scalar(player->forw, -(speed / 3));
		moved = 1;
	}
	return (moved);
}

void	ft_player_physics(t_player *player, t_map *map)
{
	if (player->move(player, player->speed))
		player->pos = ft_vec3_add(player->pos, player->to_move);
	player->rotate(player, player->rotation_angle);
	// ft_apply_player_physics(player);
	(void)map;
}