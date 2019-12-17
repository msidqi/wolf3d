#include "wolf3d.h"
#include <limits.h>

void	ft_putcharr(char c)
{
	write(1, &c, 1);
}


void	ft_putnbrr(int nb)
{
	unsigned int x;

	if (nb < 0)
	{
		ft_putcharr('-');
		x = nb * -1;
	}
	else
	{
		x = nb;
	}
	if (x >= 10)
	{
		ft_putnbrr(x / 10);
		ft_putcharr(x % 10 + '0');
	}
	else
	{
		ft_putcharr(x + '0');
	}
}

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
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
				break;
			case SDL_SCANCODE_J:
				player->forw = (t_vec3){1, 0, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
				break;
			case SDL_SCANCODE_Y:
				player->forw = (t_vec3){0, -1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
				break;
			case SDL_SCANCODE_H:
				player->forw = (t_vec3){0, 1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
				break;
			case SDL_SCANCODE_U:
				player->forw = (t_vec3){1, -1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
				break;
			case SDL_SCANCODE_T:
				player->forw = (t_vec3){-1, -1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
				break;
			case SDL_SCANCODE_M:
				player->forw = (t_vec3){1, 1, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
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
	player->pos = (t_vec3){ 350, 370, 0 };
	player->forw = ft_vec3_normalize((t_vec3){ 1, 0, 0 });
	player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, (t_vec3){ 0, 0, -1}));
	// printf("right: %f, %f, %f\n" , player->right.x, player->right.y, player->right.z);
	player->focal_len = 1;
	player->cam_hor = 1;
	player->cam_ver = 1;
	return (player);
}

double	ft_limit_inter(t_vec3 position, double to_add)
{
	double total;

	total = position.x + to_add;
	if (total > INT_MAX || total < -MICRO)
		return (INT_MAX);
	return (total);
}

t_vec3	ft_limit_inter_by_map(t_vec3 position, t_map *map)
{
	if ((position.x < (map->width * TILE_WIDTH) && position.x >= 0)
		&& (position.y < (map->height * TILE_HEIGHT) && position.y >= 0))
		return (position);
		// printf("ERROR: HERE\n");
	return ((t_vec3){ 1, 1, 1});
}

void	ft_find_vertical_intersection(t_ray *ray, t_player *player, t_vec3 mapped_pos, t_map *map)
{
	ray->dir = ft_vec3_normalize(ft_vec3_sub(mapped_pos, player->pos));
	ray->origin = player->pos;
	double dot = ft_vec3_dot_product(ray->dir, RIGHT);
	ray->angle = acos(dot);												// positive angle is down (clock-wise)
	// printf("ray dir %f, %f\n", ray->dir.x, ray->dir.y);

	double horizontal_edge;

	t_vec3 cross = ft_vec3_cross_product(ray->dir, RIGHT);
	if (ft_vec3_dot_product(FORW, cross) < 0)
		ray->angle = -ray->angle;
	if ((dot < 0))
		horizontal_edge =  player->pos.x - ((int)player->pos.x % TILE_WIDTH);		// left edge
	else
		horizontal_edge = player->pos.x + (TILE_WIDTH - (int)player->pos.x % TILE_WIDTH);	// right edge
	// printf("angle in rad: %f | angle in deg: %f |  tan(ray->angle) %f | horizontal_edge %f\n", ray->angle, ray->angle * 180 / PI , tan(ray->angle), horizontal_edge);
	// printf("current.x (%f)   |   right_edge (x): (%f)\n", player->pos.x, horizontal_edge);
	double distance_to_horizontal_edge = horizontal_edge - player->pos.x;
	// printf("distance_to_horizontal_edge: %f\n", fabs(distance_to_horizontal_edge));
	double distance_from_horizontal_edge_to_vertical_inter = distance_to_horizontal_edge * tan(ray->angle);
	// printf("distance_from_horizontal_edge_to_vertical_inter: %f\n", fabs(distance_from_horizontal_edge_to_vertical_inter));
	(void)distance_from_horizontal_edge_to_vertical_inter;



	ray->first_inter_point = ft_limit_inter_by_map((t_vec3){player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter, 0}, map);
	// if (ray->first_inter_point.x >= 100 && ray->first_inter_point.x < 110)
// printf("vertical inter point: (%f, %f)\n", player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter);
	// printf("ERROR y: %f\n", player->pos.y + distance_from_horizontal_edge_to_vertical_inter);
	//(t_vec3){player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter, 0};
	double next_horiz_edge_to_next_vertical_inter = TILE_WIDTH * tan(ray->angle); // steps
// printf("distance to first VERTICAL inter point:       (%f)\n", ft_vec3_mag(ft_vec3_sub(ray->first_inter_point, player->pos)));//ft_vec3_mag_cmp());

	ray->increments.x = dot > 0 ? TILE_WIDTH : -TILE_WIDTH;
	ray->increments.y = dot > 0 ? next_horiz_edge_to_next_vertical_inter : -next_horiz_edge_to_next_vertical_inter;

	// ray->wall_inter = ft_limit_inter_by_map(ft_vec3_add(ray->first_inter_point, ray->increments), map);
// printf("VERTICAL wall distance ------>:       (%f)\n", ft_vec3_mag(ft_vec3_sub(ray->wall_inter, player->pos)));//ft_vec3_mag_cmp());
	// if (ray->wall_inter.y >= 100 && ray->wall_inter.y < 110)
// printf("NEXT vertical inter point:       (%f, %f)\n", ray->wall_inter.x, ray->wall_inter.y);
	(void)map;
}

void	ft_find_horizontal_intersection(t_ray *ray, t_player *player, t_vec3 mapped_pos, t_map *map)
{
	t_vec3 dir = ft_vec3_normalize(ft_vec3_sub(mapped_pos, player->pos));
	// t_vec3 origin = player->pos;
	double dot = ft_vec3_dot_product(ray->dir, DOWN); // positive y (to know which edge to pick)
	double angle = acos(dot);											// positive angle is left (clock-wise)
	// printf("ray dir %f, %f\n", ray->dir.x, ray->dir.y);

	double vertical_edge;

	// if (ray->angle > (double)1.5708)
	// 	ray->angle = (double)3.14159 - ray->angle;
	t_vec3 cross = ft_vec3_cross_product(dir, DOWN);
	if (ft_vec3_dot_product(FORW, cross) > 0)
		angle = -angle;
// printf(" angle in deg: %f |  ft_vec3_dot_product(FORW, cross) %f \n", ray->angle * 180 / PI , ft_vec3_dot_product(FORW, cross));
	// printf("angle in rad: %f | angle in deg: %f \n", ray->angle, ray->angle * 180 / PI);
	if ((dot < 0))
		vertical_edge =  player->pos.y - ((int)player->pos.y % TILE_HEIGHT);		// top edge
	else
		vertical_edge = player->pos.y + (TILE_HEIGHT - (int)player->pos.y % TILE_HEIGHT);	// bottom edge
		// printf("angle in rad: %f | angle in deg: %f |  tan(ray->angle) %f | vertical_edge %f\n", ray->angle, ray->angle * 180 / PI , tan(ray->angle), vertical_edge);
	// printf("current.y (%f)   |   bottom_edge (y): (%f)\n", player->pos.y, bottom_edge);
	double distance_to_vertical_edge = vertical_edge - player->pos.y;
	// printf("distance_to_vertical_edge: %f\n", distance_to_vertical_edge);
	double distance_from_vertical_edge_to_horizontal_inter = distance_to_vertical_edge * tan(angle);
		// printf("distance_from_vertical_edge_to_horizontal_inter: %f\n", distance_from_vertical_edge_to_horizontal_inter);
// printf("horizontal inter point: (%f, %f)\n", player->pos.x + distance_from_vertical_edge_to_horizontal_inter, player->pos.y + distance_to_vertical_edge);
	(void)distance_from_vertical_edge_to_horizontal_inter;



	t_vec3 first_inter_point = ft_limit_inter_by_map((t_vec3){ player->pos.x + distance_from_vertical_edge_to_horizontal_inter, player->pos.y + distance_to_vertical_edge, 0 }, map);
	double next_edge_to_next_vertical_inter = 100 * tan(angle); // steps
	
	t_vec3 increments;
	increments.x = dot > 0 ? next_edge_to_next_vertical_inter : -next_edge_to_next_vertical_inter; ////// next inter point is weird af
	increments.y = dot > 0 ? TILE_HEIGHT : -TILE_HEIGHT;
									// printf("distance to first HORIZONTAL inter point:       (%f)\n", ft_vec3_mag(ft_vec3_sub(first_inter_point, player->pos)));//ft_vec3_mag_cmp());
	
	if (ft_vec3_mag(ft_vec3_sub(ray->first_inter_point, player->pos)) > ft_vec3_mag(ft_vec3_sub(first_inter_point, player->pos)))
	{
		ray->angle = angle;
		ray->dir = dir;
		ray->origin = player->pos;
		ray->first_inter_point = first_inter_point;
		ray->increments = increments;
	}

	// ray->wall_inter = ft_limit_inter_by_map(ft_vec3_add(ray->first_inter_point, ray->increments), map);
// printf("HORIZONTAL wall distance ------>:       (%f)\n", ft_vec3_mag(ft_vec3_sub(ray->wall_inter, player->pos)));//ft_vec3_mag_cmp());
// printf("NEXT horizontal edge inter point:       (%f, %f)\n", wall_inter.x, wall_inter.y);
	(void)map;
	(void)first_inter_point;
}

t_index get_map_index(t_vec3 position, t_map *map)
{
	// printf("x: %f y: %.14f\n", position.x, position.y);
	if (position.x >= -MICRO && position.y >= -MICRO && position.y < (map->height * 100) && position.x < (map->width * 100))
		return ((t_index){ (int)(position.x / 100), (int)(position.y / 100) });
	return ((t_index){ -1, -1 });
}

t_tile *get_map_tile(t_index m_index, t_map *map)
{
	// printf("index.y : %d, height: %d, index.x : %d, width : %d\n", m_index.y, map->height, m_index.x, map->width);
	if (m_index.x >= 0 && m_index.y >= 0 && m_index.y < map->height && m_index.x < map->width)
		return (&map->tiles[m_index.y][m_index.x]);
	return (NULL);
}

void ft_find_closest_wall(t_ray *ray, t_map *map, t_player *player)
{
	t_vec3 i;
	t_index m_index;
	t_tile *tile;
	i = ray->first_inter_point;
	m_index = get_map_index(i, map);
	// t_index player_index = get_map_index(player->pos, map);
	// printf("m_indexx: %d m_indexy: %d\n", m_index.x, m_index.y);
	if (m_index.x < 0 || m_index.y < 0 || !(tile = get_map_tile(m_index, map)))
	{
		printf("first inter out of array !! \n");
		exit(1);
	}
	if (tile != NULL && tile->depth == 1)
		printf("first inter is a wall-----inter at: (%f, %f)-----------------------> Array [%d, %d]\n", i.x, i.y, m_index.x, m_index.y);

	// printf("In map tiles, intersection is at --------------(%f, %f)-----------------------> Array [%d, %d]\n", i.x, i.y, m_index.x, m_index.y);
	// t_tile *tile_ptr = get_map_tile(pos_in_array, map);

// i = ft_vec3_add(ray->first_inter_point, ray->increments);
// m_index = get_map_index(i, map);
// if (m_index.x < 0 || m_index.y < 0 || !get_map_tile(m_index, map))
// {
// 	printf("wall_inter out of array !! \n");
// 	exit(1);
// }
	// printf("In map tiles, intersection is at --------------(%f, %f)-----------------------> Array [%d, %d]\n\n", i.x, i.y, m_index.x, m_index.y);
	// printf("NEXT right edge inter point:       (%f, %f)\n", i.x, i.y);
	// while ()
	// {

	// }


	// if (tile_ptr)
	// 	printf("Tile -------------------> (%f, %f) depth: %.1f\n", inter.x, inter.y, (*tile_ptr).depth);
	// printf("WALL AT -------------------> (%f, %f)\n", inter.x, inter.y);
	// (void)tile_ptr;
	(void)ray;
	(void)map;
	(void)player;
}

void	ft_ray_cast(t_player *player, t_map *map, SDL_Surface *surface)
{
	int x;
	t_vec3	mapped_pos;
	t_ray ray;

	x = -1;
	while (++x < WIDTH)
	{
		mapped_pos = ft_map_pixels_to_world(x, player);
		ft_find_vertical_intersection(&ray, player, mapped_pos, map);
		ft_find_horizontal_intersection(&ray, player, mapped_pos, map);
		//FIRST
		if (ray.first_inter_point.x > 0 && ray.first_inter_point.y > 0 && ray.first_inter_point.x <= map->width * TILE_WIDTH && ray.first_inter_point.y <= map->height * TILE_HEIGHT)
		{
			put_pixel32(surface, ray.first_inter_point.x, ray.first_inter_point.y, 0xFFFFFF00);
			put_pixel32(surface, ray.first_inter_point.x + 1, ray.first_inter_point.y, 0xFFFFFF00);
			put_pixel32(surface, ray.first_inter_point.x, ray.first_inter_point.y + 1, 0xFFFFFF00);
			put_pixel32(surface, ray.first_inter_point.x - 1, ray.first_inter_point.y, 0xFFFFFF00);
			put_pixel32(surface, ray.first_inter_point.x, ray.first_inter_point.y - 1, 0xFFFFFF00);
		}
		ft_find_closest_wall(&ray, map, player);
		//SECOND
		// if (ray.wall_inter.x > 0 && ray.wall_inter.y > 0 && ray.wall_inter.x <= map->width * TILE_WIDTH && ray.wall_inter.y <= map->height * TILE_HEIGHT)
		// {
		// 	put_pixel32(surface, ray.wall_inter.x, ray.wall_inter.y, 0xFFFFFF00);
		// 	put_pixel32(surface, ray.wall_inter.x + 1, ray.wall_inter.y, 0xFFFFFF00);
		// 	put_pixel32(surface, ray.wall_inter.x, ray.wall_inter.y + 1, 0xFFFFFF00);
		// 	put_pixel32(surface, ray.wall_inter.x - 1, ray.wall_inter.y, 0xFFFFFF00);
		// 	put_pixel32(surface, ray.wall_inter.x, ray.wall_inter.y - 1, 0xFFFFFF00);
		// }
		put_pixel32(surface, player->pos.x, player->pos.y, 0xFF00FFFF);
		// break ;
		// ft_draw_intersection_point();
	}
	// exit(1);
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
	win = ft_sdl_init_create_window(500, 400, 1600, 720);
	bmp = ft_create_surface(win, WIN_WIDTH, 521, BPP);
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
		}
		// ft_print_map(map, player);
		ft_draw_map(bmp, map, player);
		ft_ray_cast(player, map, bmp);
		SDL_BlitSurface(bmp, NULL, display, NULL);
		SDL_UpdateWindowSurface(win);
	}
	ft_destroy_map(map);
	SDL_FreeSurface(bmp);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
