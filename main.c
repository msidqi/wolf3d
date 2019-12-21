#include "wolf3d.h"

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

	// int i;

	// i = MACRO_FUNCTIONNN;
	if( event.type == SDL_KEYDOWN)// && event.key.repeat == 0 )
		switch (scan_code)
		{
			case SDL_SCANCODE_D:
				player->pos = ft_vec3_add(player->right, player->pos);
				break;
			case SDL_SCANCODE_A:
				player->pos = ft_vec3_add(ft_vec3_scalar(player->right, -1), player->pos);
				break;
			case SDL_SCANCODE_W:
				player->pos = ft_vec3_add(player->forw, player->pos);
				break;
			case SDL_SCANCODE_S:
				player->pos = ft_vec3_add(ft_vec3_scalar(player->forw, -1), player->pos);
				break;
			case SDL_SCANCODE_LEFT:
				player->forw = (t_vec3){
					cos(-ROTATION_ANGLE) * player->forw.x - sin(-ROTATION_ANGLE) * player->forw.y,
					sin(-ROTATION_ANGLE) * player->forw.x + cos(-ROTATION_ANGLE) * player->forw.y
					, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_RIGHT:
				player->forw = (t_vec3){
					cos(ROTATION_ANGLE) * player->forw.x - sin(ROTATION_ANGLE) * player->forw.y,
					sin(ROTATION_ANGLE) * player->forw.x + cos(ROTATION_ANGLE) * player->forw.y
					, 0};
				player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
				break;
			case SDL_SCANCODE_UP:
				player->pos = ft_vec3_add(player->forw, player->pos);
				break;
			case SDL_SCANCODE_DOWN:
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
	// printf("x: %f | y: %f\n", player->pos.x, player->pos.y);
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

	x_world = player->cam_hor / BMP_WIDTH * (x - BMP_WIDTH / 2);
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
	player->pos = (t_vec3){ 505, 310, 0 };
	player->forw = ft_vec3_normalize((t_vec3){ -1, 0, 0 });
	player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
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

t_index	ft_limit_index_by_map(t_index index, t_map *map)
{
	if ((index.x < (map->width) && index.x >= 0)
		&& (index.y < (map->height) && index.y >= 0))
		return (index);
		// printf("ERROR: HERE\n");
	return ((t_index){ 1, 1});
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



	ray->first_ver_point = ft_limit_inter_by_map((t_vec3){player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter, 0}, map);
	// if (ray->first_inter_point.x >= 100 && ray->first_inter_point.x < 110)
// printf("vertical inter point: (%f, %f)\n", player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter);
	// printf("ERROR y: %f\n", player->pos.y + distance_from_horizontal_edge_to_vertical_inter);
	//(t_vec3){player->pos.x + distance_to_horizontal_edge, player->pos.y + distance_from_horizontal_edge_to_vertical_inter, 0};
	double next_horiz_edge_to_next_vertical_inter = TILE_WIDTH * tan(ray->angle); // steps
// printf("distance to first VERTICAL inter point:       (%f)\n", ft_vec3_mag(ft_vec3_sub(ray->first_inter_point, player->pos)));//ft_vec3_mag_cmp());

	ray->increments_v.x = dot > 0 ? TILE_WIDTH : -TILE_WIDTH;
	ray->increments_v.y = dot > 0 ? next_horiz_edge_to_next_vertical_inter : -next_horiz_edge_to_next_vertical_inter;

	// ray->wall_inter = ft_limit_inter_by_map(ft_vec3_add(ray->first_inter_point, ray->increments), map);
// printf("VERTICAL wall distance ------>:       (%f)\n", ft_vec3_mag(ft_vec3_sub(ray->wall_inter, player->pos)));//ft_vec3_mag_cmp());
	// if (ray->wall_inter.y >= 100 && ray->wall_inter.y < 110)
// printf("NEXT vertical inter point:       (%f, %f)\n", ray->wall_inter.x, ray->wall_inter.y);
	(void)map;
}

void	ft_find_horizontal_intersection(t_ray *ray, t_player *player, t_vec3 mapped_pos, t_map *map)
{
	ray->dir = ft_vec3_normalize(ft_vec3_sub(mapped_pos, player->pos));
	// t_vec3 origin = player->pos;
	double dot = ft_vec3_dot_product(ray->dir, DOWN); // positive y (to know which edge to pick)
	ray->angle = acos(dot);											// positive angle is left (clock-wise)
	// printf("ray dir %f, %f\n", ray->dir.x, ray->dir.y);

	double vertical_edge;

	// if (ray->angle > (double)1.5708)
	// 	ray->angle = (double)3.14159 - ray->angle;
	t_vec3 cross = ft_vec3_cross_product(ray->dir, DOWN);
	if (ft_vec3_dot_product(FORW, cross) > 0)
		ray->angle = -ray->angle;
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
	double distance_from_vertical_edge_to_horizontal_inter = distance_to_vertical_edge * tan(ray->angle);
		// printf("distance_from_vertical_edge_to_horizontal_inter: %f\n", distance_from_vertical_edge_to_horizontal_inter);
// printf("horizontal inter point: (%f, %f)\n", player->pos.x + distance_from_vertical_edge_to_horizontal_inter, player->pos.y + distance_to_vertical_edge);
	(void)distance_from_vertical_edge_to_horizontal_inter;



	ray->first_hor_point = ft_limit_inter_by_map((t_vec3){ player->pos.x + distance_from_vertical_edge_to_horizontal_inter, player->pos.y + distance_to_vertical_edge, 0 }, map);
	double next_edge_to_next_vertical_inter = TILE_HEIGHT * tan(ray->angle); // steps
	
	// t_vec3 increments;
	ray->increments_h.x = dot > 0 ? next_edge_to_next_vertical_inter : -next_edge_to_next_vertical_inter; ////// next inter point is weird af
	ray->increments_h.y = dot > 0 ? TILE_HEIGHT : -TILE_HEIGHT;
									// printf("distance to first HORIZONTAL inter point:       (%f)\n", ft_vec3_mag(ft_vec3_sub(first_inter_point, player->pos)));//ft_vec3_mag_cmp());
	
	// if (ft_vec3_mag(ft_vec3_sub(ray->first_inter_point, player->pos)) > ft_vec3_mag(ft_vec3_sub(first_inter_point, player->pos)))
	// {
	// 	ray->angle = ray->angle;
	// 	ray->dir = ray->dir;
	// 	ray->origin = player->pos;
	// 	ray->first_inter_point = first_inter_point;
	// 	ray->increments = increments;
	// }

	// ray->wall_inter = ft_limit_inter_by_map(ft_vec3_add(ray->first_inter_point, ray->increments), map);
// printf("HORIZONTAL wall distance ------>:       (%f)\n", ft_vec3_mag(ft_vec3_sub(ray->wall_inter, player->pos)));//ft_vec3_mag_cmp());
// printf("NEXT horizontal edge inter point:       (%f, %f)\n", wall_inter.x, wall_inter.y);
	(void)map;
}

t_index get_map_index(t_vec3 position, t_map *map)
{
	// printf("x: %f y: %.14f\n", position.x, position.y);
	if (position.x >= -MICRO && position.y >= -MICRO && position.y < (map->height * TILE_HEIGHT) && position.x < (map->width * TILE_WIDTH))
		return ((t_index){ (int)(position.x / TILE_WIDTH), (int)(position.y / TILE_HEIGHT) });
	return ((t_index){ -1, -1 });
}

t_tile *get_map_tile_ver(t_index m_index, t_map *map)
{
	if (m_index.x >= 0 && m_index.y >= 0 && m_index.y < map->height && m_index.x < map->width)
	{
		if (map->tiles[m_index.y][m_index.x].depth)
			return (&map->tiles[m_index.y][m_index.x]);
		if (((m_index.x - 1) >= 0 && (m_index.x - 1) < map->width))//		&& map->tiles[m_index.y - 1][m_index.x].depth)
			return (&map->tiles[m_index.y][m_index.x - 1]);
	}
	return (NULL);
}

t_tile *get_map_tile_hor(t_index m_index, t_map *map)
{
	if (m_index.x >= 0 && m_index.y >= 0 && m_index.y < map->height && m_index.x < map->width)
	{
		if (map->tiles[m_index.y][m_index.x].depth)
			return (&map->tiles[m_index.y][m_index.x]);
		if (((m_index.y - 1) >= 0 && (m_index.y - 1) < map->height))//		&& map->tiles[m_index.y - 1][m_index.x].depth)
			return (&map->tiles[m_index.y - 1][m_index.x]);
	}
	return (NULL);
}

t_wall ft_find_closest_wall(t_ray *ray, t_map *map, t_player *player, t_vec3 mapped_pos, SDL_Surface *bmp)
{
	t_tile *tile = &map->tiles[0][0];
	t_tile *tile2 = &map->tiles[0][0];
	// double distance;
	// double distance2;
	t_vec3 i = ray->first_hor_point;
	t_vec3 j = ray->first_ver_point;
	// t_vec3 i2;
	// t_vec3 j2;
	// int c = 0x0;
	// int c2 = 0x0;
	// int draw;
	// t_inter	hor[3];
	// t_inter	ver[3];
	// for (size_t p = 0; p < 3; p++)
	// {
	// 	ver[p].distance = -1;
	// 	hor[p].distance = -1;
	// }
	
	t_inter	hor;
	t_inter	ver;
	hor.intersected = 0;
	ver.intersected = 0;
	hor.distance = MEGA;
	ver.distance = MEGA;
	// int counter_h = 0;
	// int counter_v = 0;
	while ((tile || tile2) && (!hor.intersected || !ver.intersected))
	{
		// draw = 0;
		// distance = MEGA;
		// distance2 = MEGA;
		if (!hor.intersected && (tile = get_map_tile_hor( get_map_index(i, map), map )))
		{
			if (tile->depth == 1)
			{
				hor.distance = ft_vec3_mag(ft_vec3_sub(i, player->pos));
				// hor[counter_h].distance = distance;
				// hor[counter_h].point = i;
				// put_pixel32(surface, i.x, i.y, 0xFFFF0000  + c);
				// c += 0x5599;
				// printf("distance to hor %f\n", hor.distance);
				hor.facing = (i.y / TILE_HEIGHT == tile->index.y) ? NORTH : SOUTH;
				// printf("tile %d, %d | facing %d\n", tile->index.x, tile->index.y, hor.facing);
				// draw = 1;
				hor.point = i;
				hor.intersected = 1;
				tile = NULL;
			}
			i = ft_vec3_add(i, ray->increments_h);
		}
		if (!ver.intersected && (tile2 = get_map_tile_ver( get_map_index(j, map), map )))
		{
			if (tile2->depth == 1)
			{
				ver.distance = ft_vec3_mag(ft_vec3_sub(j, player->pos));
				// ver[counter_v].distance = distance2;
				// ver[counter_v].point = j;
				// put_pixel32(surface, j.x, j.y, 0xFF00FF00 + c2);
				// c2 += 0x330099;
				// printf("distance to ver %f\n", ver.distance);
				// draw = ((distance2 <= distance) || !tile) ? 2 : 1;
				// printf("tile2 %d, %d\n", tile2->index.x, tile2->index.y);
				ver.facing = (j.x / TILE_WIDTH == tile2->index.x) ? WEST : EAST;
				// printf("tile2 %d, %d | facing %d\n", tile2->index.x, tile2->index.y, ver.facing);
				ver.point = j;
				ver.intersected = 1;
				tile2 = NULL;
			}
			// if (tile->depth == 1)
			// 	put_pixel32(surface, i.x, i.y, 0xFFFF0000);
			j = ft_vec3_add(j, ray->increments_v);
		}
	}
	t_wall	wall;
	wall.facing = SKYBOX;
	if (hor.distance < ver.distance)
	{
		ray->wall_inter = hor.point;
		wall.inter_point = hor.point;
		wall.distance_from_origin = hor.distance;
		wall.facing = hor.facing;
		put_pixel32(bmp, hor.point.x / MINI_MAP_RATIO_WIDTH, hor.point.y / MINI_MAP_RATIO_HEIGHT, 0xFFFF0000);
		// printf("inter %f, %f\n", wall.inter_point.x, wall.inter_point.y);
	}
	else if (ver.distance >= ver.distance && ver.distance != MEGA)
	{
		ray->wall_inter = ver.point;
		wall.inter_point = ver.point;
		wall.distance_from_origin = ver.distance;
		wall.facing = ver.facing;
		put_pixel32(bmp, ver.point.x / MINI_MAP_RATIO_WIDTH, ver.point.y / MINI_MAP_RATIO_HEIGHT, 0xFF00FF00);
		// printf("inter %f, %f\n", wall.inter_point.x, wall.inter_point.y);
	}
	// for (int c = 0; c < 3; c++)
	// {
	// 	if (c < counter_v)
	// 		printf("distance to hor %f\n", hor[c].distance);
	// 	if (c < counter_h)
	// 		printf("distance to ver %f\n", ver[c].distance);
	// 	printf("-------\n");
	// }
	
		// printf("distance to hor %f distance to ver %f\n", distance, distance2);//ft_vec3_mag(ft_vec3_sub(i, player->pos)), ft_vec3_mag(ft_vec3_sub(j, player->pos)));
	(void)bmp;
	(void)mapped_pos;
	(void)player;
	return (wall);
}

/*
**	initializes textures on first call only
**	returns an array of SDL surfaces whenever called
*/

SDL_Surface **get_all_textures()
{
	static int initialized = 0;
	static SDL_Surface *game_textures[TEXTURE_NUM];

	if (!initialized)
	{
		game_textures[0] = IMG_Load("./Textures/NORTH.JPG");
		game_textures[1] = IMG_Load("./Textures/SOUTH.JPG");
		game_textures[2] = IMG_Load("./Textures/EAST.JPG");
		game_textures[3] = IMG_Load("./Textures/WEST.JPG");
		// IMG_Load("SKY.JPG"),
		// IMG_Load("GROUND.JPG"),
		initialized = 1;
	}
	return (game_textures);
}

int get_wall_texture(int x, int y, t_wall wall)
{
	int color;
	
	color = 0x44C3EE91;
	if (wall.facing == NORTH)
		color = 0xFFe8e8e8;
	else if (wall.facing == SOUTH)
		color = 0xFF75161f;
	else if (wall.facing == EAST)
		color = 0xFFfffa7f;
	else if (wall.facing == WEST)
		color = 0xFFcba8ee;
	// else if (wall.facing == SKYBOX)
	// 	color = 0xFF9eff00;
	(void)x;
	(void)y;
	return (color);
}

void ft_draw_walls(int x, t_wall wall, SDL_Surface *bmp)
{
	int i;
	int j;
	int max_pixels;
	int counter;
	SDL_Surface **textures;
	textures = get_all_textures();

	if (wall.facing == SKYBOX)
		return ;// draw skybox
	i = bmp->h / 2;
	j = i + 1;
	max_pixels = (int)(bmp->h * 2 / wall.distance_from_origin + 20);
	counter = 0;
	while (counter < max_pixels)
	{
		// printf("counter\n");
		if (i >= 0)
		{
			if (i == bmp->h / 2)
				printf("x: %f | y: %f | t_width: %d | t_height %d\n", (double)x / BMP_WIDTH, (double)i / bmp->h, textures[wall.facing]->w, textures[wall.facing]->h);
			// printf("%d | %d\n", (int)(((double)x / BMP_WIDTH) * textures[wall.facing]->w), (int)(((double)i / bmp->h) * textures[wall.facing]->h) );
			// printf("%X\n", get_pixel32(textures[SOUTH], 50, 10));
			put_pixel32(bmp, x, i, get_pixel32(textures[SOUTH],
			(int)(((double)x / BMP_WIDTH) * textures[SOUTH]->w),
			(int)(((double)i / bmp->h) * textures[SOUTH]->h)));//, get_wall_texture(x, i, wall)
			i--;
		}
		if (j < bmp->h)
		{
			put_pixel32(bmp, x, j, get_wall_texture(x, j, wall));
			j++;
		}
		counter++;
	}
	// exit(1);
	(void)x;
	(void)bmp;
	(void)wall;
}

void	ft_ray_cast(t_player *player, t_map *map, SDL_Surface *bmp)
{
	int x;
	t_vec3	mapped_pos;
	t_ray ray;
	t_wall wall;

	x = -1;
	while (++x < BMP_WIDTH)
	{
		mapped_pos = ft_map_pixels_to_world(x, player);
		ft_find_vertical_intersection(&ray, player, mapped_pos, map);
		ft_find_horizontal_intersection(&ray, player, mapped_pos, map);

		wall = ft_find_closest_wall(&ray, map, player, mapped_pos, bmp);
		if (wall.facing != SKYBOX && wall.facing != GROUND)
			ft_draw_walls(x, wall, bmp);
	}
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
	win = ft_sdl_init_create_window(500, 400, WIN_WIDTH, WIN_HEIGHT);
	bmp = ft_create_surface(win, BMP_WIDTH, BMP_HEIGHT, BPP);
	display = SDL_GetWindowSurface(win);
	map = ft_create_map(10, 5);
	//Initialize PNG loading
	if(!( IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG) || !get_all_textures())
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		exit(1);
	}
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			// printf("\e[1;1H\e[2J");
			if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				quit = true;
			ft_player_controller(player, event);
			// printf("playerPos: %f, %f\nscanCode %d | keyCode %d \n", player->pos.x, player->pos.y, scan_code_down(event), key_code(event));
		}
		// ft_print_map(map, player);
		ft_draw_map(bmp, map, player);
		ft_ray_cast(player, map, bmp);
		SDL_BlitSurface(bmp, NULL, display, NULL);
		// SDL_BlitSurface(textures[3].img, NULL, display, NULL);
		SDL_UpdateWindowSurface(win);
		SDL_FillRect(bmp, NULL, 0x000000);
		SDL_FillRect(display, NULL, 0x000000);
	}
	ft_destroy_map(map);
	SDL_FreeSurface(bmp);
	int k = -1;
	SDL_Surface **array_to_free = get_all_textures();
	while (++k < TEXTURE_NUM)
		SDL_FreeSurface(array_to_free[k]);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
