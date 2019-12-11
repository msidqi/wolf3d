#include "wolf3d.h"

void	ft_player_controller(t_player *player, SDL_Event event)
{
	Uint32 scan_code = scan_code_down(event);

	if( event.type == SDL_KEYDOWN)// && event.key.repeat == 0 )
		switch (scan_code)
		{
			case SDL_SCANCODE_LEFT:
				player->pos.x -= 1;
				break;
			case SDL_SCANCODE_RIGHT:
				player->pos.x += 1;
				break;
			case SDL_SCANCODE_UP:
				player->pos.y -= 1;
				break;
			case SDL_SCANCODE_DOWN:
				player->pos.y += 1;
				break;
			case SDL_SCANCODE_G:
				player->forw = (t_vec3){-1, 0, 0};
				break;
			case SDL_SCANCODE_J:
				player->forw = (t_vec3){1, 0, 0};
				break;
			case SDL_SCANCODE_Y:
				player->forw = (t_vec3){0, -1, 0};
				break;
			case SDL_SCANCODE_H:
				player->forw = (t_vec3){0, 1, 0};
				break;
			case SDL_SCANCODE_U:
				player->forw = (t_vec3){1, -1, 0};
				break;
			case SDL_SCANCODE_T:
				player->forw = (t_vec3){-1, -1, 0};
				break;
			case SDL_SCANCODE_M:
				player->forw = (t_vec3){1, 1, 0};
				break;
			case SDL_SCANCODE_N:
				player->forw = (t_vec3){-1, 1, 0};
				break;
		}
	printf("x: %f | y: %f\n", player->pos.x, player->pos.y);
}

int	game_manager(t_action *actions)
{
	(void)actions;
	// if (player's action is OK)
	// 	return (1);
	return (0);
}

t_vec3	ft_map_pixels_to_world(int x, t_player *player)
{
	double	x_world;
	t_vec3	from_player_to_frame;
	t_vec3	from_frame_center_to_right;
	t_vec3	mapped_pos;

	x_world = player->cam_hor / WIDTH * (x - WIDTH / 2);
	from_player_to_frame = ft_vec3_scalar(player->forw, player->focal_len);
	from_frame_center_to_right = ft_vec3_scalar(player->right, x_world);///

	mapped_pos = ft_vec3_add(player->pos,
		ft_vec3_add(from_player_to_frame, from_frame_center_to_right));
	// printf("x: %f, y: %f\n",mapped_pos.x, mapped_pos.y);
	return (mapped_pos);
}

t_vec3	ft_vec3_cross_product(t_vec3 vec1, t_vec3 vec2)
{
	t_vec3 ret;

	ret.x = vec1.y * vec2.z - vec1.z * vec2.y;
	ret.y = vec1.z * vec2.x - vec1.x * vec2.z;
	ret.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return (ret);
}

t_player *ft_create_player()
{
	t_player *player;

	player = (t_player *)malloc(sizeof(t_player));
	player->pos = (t_vec3){ 100, 100, 0 };
	player->forw = (t_vec3){ 1, 0, 0 };
	player->right = (t_vec3){ 0, 1, 0 };
	player->focal_len = 1;
	player->cam_hor = 1;
	player->cam_ver = 1;
	return (player);
}

void	ft_send_ray(t_ray *ray, t_player *player, t_vec3 mapped_pos)
{
	ray->dir = ft_vec3_normalize(ft_vec3_sub(mapped_pos, player->pos));
	ray->origin = player->pos;
	double dot = ft_vec3_dot_product(ray->dir, RIGHT);
	ray->angle = acos(dot);

	double horizontal_edge;


	t_vec3 cross = ft_vec3_cross_product(ray->dir, RIGHT);
	if (ft_vec3_dot_product(FORW, cross) < 0)
		ray->angle = -ray->angle;
	// if ((dot < 0))
	// 	horizontal_edge =  player->pos.x - ((int)player->pos.x % 100);		// left edge
	// else
		horizontal_edge = player->pos.x + (100 - (int)player->pos.x % 100);	// right edge
	// printf("ray->dir: (%f, %f)\n", ray->dir.x, ray->dir.y);
	// printf("angle in deg: %f\n", ray->angle/M_PI * 180);
	// printf("angle in rad: %f |  tan(ray->angle) %f\n", ray->angle,  tan(ray->angle));
	// printf("current.x (%f)   |   right_edge (x): (%f)\n", player->pos.x, horizontal_edge);
	double distance_to_horizontal_edge = horizontal_edge - player->pos.x;
	// printf("distance_to_horizontal_edge: %f\n", distance_to_horizontal_edge);
	double distance_from_horizontal_edge_to_vertical_inter = distance_to_horizontal_edge * tan(ray->angle);
	// printf("distance_from_horizontal_edge_to_vertical_inter: %f\n", distance_from_horizontal_edge_to_vertical_inter);
	printf("right edge inter point: (%f, %f)\n", player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter);
	(void)distance_from_horizontal_edge_to_vertical_inter;



	t_vec3 first_inter_point = (t_vec3){player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter, 0};
	double next_horiz_edge_to_next_vertical_inter = 100 * tan(ray->angle); // steps
	printf("NEXT right edge inter point: (%f, %f)\n", first_inter_point.x + 100, first_inter_point.y + next_horiz_edge_to_next_vertical_inter);
}

void	ft_ray_cast(t_player *player, t_map *map)
{
	int x;
	t_vec3	mapped_pos;
	t_ray ray;
	(void)map;

	x = -1;
	while (++x < WIDTH)
	{
		mapped_pos = ft_map_pixels_to_world(x, player);
		ft_send_ray(&ray, player, mapped_pos);
		// float angle
		// printf("%f, %f, %f\n", mapped_pos.x, mapped_pos.y, mapped_pos.z);
	}
	// ft_send_ray(&ray, player);
	exit(1);
}

int	main()
{
	SDL_Window	*win;
	SDL_Surface	*bmp;
	SDL_Event	event;
	SDL_Surface *display;
	t_map		*map;
	t_player	*player;
	int			quit;
	
	quit = false;
	player = ft_create_player();
	win = ft_sdl_init_create_window(10, 10, 1280, 720);
	bmp = ft_create_surface(win, WIDTH, HEIGHT, BPP);
	display = SDL_GetWindowSurface(win);
	map = ft_create_map(10, 5);
	
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			printf("\e[1;1H\e[2J");
			if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				quit = true;
			ft_player_controller(player, event);
			printf("playerPos: %f, %f\nscanCode %d | keyCode %d \n", player->pos.x, player->pos.y, scan_code_down(event), key_code(event));
			ft_print_map(map, player);
		}
		ft_ray_cast(player, map);
		SDL_BlitSurface(bmp, NULL, display, NULL);
		SDL_UpdateWindowSurface(win);
	}
	ft_destroy_map(map);
	SDL_FreeSurface(bmp);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
