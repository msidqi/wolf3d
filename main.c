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
	(void)mapped_pos;
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
	(void)mapped_pos;
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
	double cos_alpha;

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
				cos_alpha = ft_vec3_dot_product(player->forw, ray->dir);
				hor.distance = ft_vec3_mag(ft_vec3_sub(i, player->pos)) * cos_alpha;
				// hor[counter_h].distance = distance;
				// hor[counter_h].point = i;
				// put_pixel32(surface, i.x, i.y, 0xFFFF0000  + c);
				// c += 0x5599;
				// printf("distance to hor %f\n", hor.distance);
				// printf("tile %d, %d\n", tile->index.x, tile->index.y);
				hor.facing = ((int)(i.y / TILE_HEIGHT) == tile->index.y) ? NORTH : SOUTH;
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
				cos_alpha = ft_vec3_dot_product(player->forw, ray->dir);
				ver.distance = ft_vec3_mag(ft_vec3_sub(j, player->pos))  * cos_alpha;
				// ver[counter_v].distance = distance2;
				// ver[counter_v].point = j;
				// put_pixel32(surface, j.x, j.y, 0xFF00FF00 + c2);
				// c2 += 0x330099;
				// printf("distance to ver %f\n", ver.distance);
				// draw = ((distance2 <= distance) || !tile) ? 2 : 1;
				// printf("j.x %f, j.y %f\n", j.x, j.y);
				// printf("tile2 %d, %d\n", tile2->index.x, tile2->index.y);
				ver.facing = ((int)(j.x / TILE_WIDTH) == tile2->index.x) ? WEST : EAST;
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

int get_wall_texture(int x, int y, t_wall wall, SDL_Surface *bmp)
{	
	SDL_Surface **textures;
	int color;

	textures = get_all_textures();
	// color = getpixel(textures[wall.facing],
	// 		(int)(((double)x / BMP_WIDTH) * textures[wall.facing]->w),
	// 		(int)(((double)y / bmp->h) * textures[wall.facing]->h));
	color = getpixel(textures[wall.facing],
			(int)(x * textures[wall.facing]->w),
			(int)(y * textures[wall.facing]->h));
	//((double)x / BMP_WIDTH)
	//(double)y / bmp->h
	//(((int)wall.inter_point.x % 100) / TILE_WIDTH)
	// printf("color: %X\n", color);
	if (wall.facing == SKYBOX)
		return (0xFFFFFFFF);
	return (0xFF000000 + color);
	(void)x;
	(void)bmp;
}


void ft_draw_walls(int x, t_wall wall, SDL_Surface *bmp)
{
	int i;
	int j;
	int counter;
	double wall_height;

	if (wall.facing == SKYBOX)
		return ;// draw skybox
	i = bmp->h / 2;
	j = i + 1;
	// max_pixels = (int)(bmp->h / wall.distance_from_origin);// + 20)
	counter = 0;
	wall_height = (int)(bmp->h / wall.distance_from_origin);
	int txt0;
	int txt1;
	txt0 = wall_height / 2;
	txt1 = txt0 + 1;

	double mapped_x;
	if (wall.facing == NORTH || wall.facing == SOUTH)
		mapped_x = (int)wall.inter_point.x % TILE_WIDTH / TILE_WIDTH;
	else
		mapped_x = (int)wall.inter_point.y % TILE_HEIGHT / TILE_HEIGHT;
	while (counter < wall_height / 2)
	{
		if (i >= 0 && txt0 >= 0)
		{
			// wall_height = (double)i / TILE_DEPTH;
			//printf("wall_height %f\n ", wall_height);
				// printf("x: %f | y: %f | t_width: %d | t_height %d\n", (double)x / BMP_WIDTH, (double)i / bmp->h, textures[wall.facing]->w, textures[wall.facing]->h);
			// printf("%d | %d\n",
			// (int)(((double)x / BMP_WIDTH) * textures[wall.facing]->w),
			// (int)(((double)i / bmp->h) * textures[wall.facing]->h));
			// printf("mapped_x %d %d\n", mapped_x, txt0 / wall_height);
			put_pixel32(bmp, x, i, get_wall_texture(mapped_x, txt0 / wall_height, wall, bmp));//, get_wall_texture(x, i, wall)
			txt0--;
			i--;
		}
		if (j < bmp->h && txt1 < wall_height)
		{
			put_pixel32(bmp, x, j, get_wall_texture(mapped_x, txt1 / wall_height, wall, bmp));
			txt1++;
			j++;
		}
		counter++;
	}
	(void)x;
}

void ft_draw_map_wall_inter(t_wall wall, SDL_Surface *bmp)
{
	put_pixel32(bmp, wall.inter_point.x / MINI_MAP_RATIO_WIDTH, wall.inter_point.y / MINI_MAP_RATIO_HEIGHT, 0xFFFF0000);
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
		{
			ft_draw_walls(x, wall, bmp);
			ft_draw_map_wall_inter(wall, bmp);
		}
	// exit(1);
	}
}

void ft_fill_background(SDL_Surface *bmp)
{
	int x;
	int y;
	x = 0;
	while (x < bmp->w)
	{
		y = 0;
		while (y < bmp->h / 2)
		{
			 put_pixel32(bmp, x, y, 0xFF5381FF);
			 y++;
		}
		x++;
	}
	x = 0;
	while (x < bmp->w)
	{
		y = bmp->h / 2;
		while (y < bmp->h)
		{
			 put_pixel32(bmp, x, y, 0xFF717171);
			 y++;
		}
		x++;
	}
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
	// if (player->controller[PLAYER_TURN_LEFT])
	// {
	// 	player->forw =
	// 	(t_vec3){cos(-ROTATION_ANGLE) * player->forw.x - sin(-ROTATION_ANGLE) * player->forw.y,
	// 		sin(-ROTATION_ANGLE) * player->forw.x + cos(-ROTATION_ANGLE) * player->forw.y, 0};
	// 	player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
	// }
	// if (player->controller[PLAYER_TURN_RIGHT])
	// {
	// 	player->forw =
	// 	(t_vec3){cos(ROTATION_ANGLE) * player->forw.x - sin(ROTATION_ANGLE) * player->forw.y,
	// 		sin(ROTATION_ANGLE) * player->forw.x + cos(ROTATION_ANGLE) * player->forw.y, 0};
	// 	player->right = ft_vec3_normalize(ft_vec3_cross_product(player->forw, FORW));
	// }
	// ft_apply_player_physics(player);
	(void)map;
}

void	ft_apply_physics(t_player *player, t_map *map)
{
	static Uint64 previous_tick = 0;
	// Uint64 delta_time;
	
	if (!previous_tick)
		previous_tick = SDL_GetPerformanceCounter();
	// delta_time = 0;
	// delta_time = SDL_GetPerformanceCounter() - previous_tick;
		// printf("delta_time %llu\n", delta_time);
		// printf("SDL_GetPerformanceCounter() %llu | previous_tick %llu | sub : %llu \n", SDL_GetPerformanceCounter(), previous_tick, SDL_GetPerformanceCounter() - previous_tick);
	
	if (SDL_GetPerformanceCounter() - previous_tick > (Uint64)SECOND / 30)
	{
		printf("PHYSICS\n");
		ft_player_physics(player, map);
		previous_tick = SDL_GetPerformanceCounter();
		// printf("SDL_GetPerformanceFrequency() %llu\n", previous_tick);
		// printf("%u\n", SDL_GetTicks());
	}
	(void)map;
	(void)player;
}

void ft_apply_render(t_sdl_data *sdl_data, t_map *map, t_player *player)
{
	printf("ft_apply_render\n");
	ft_fill_background(sdl_data->bmp);
	ft_draw_map(sdl_data->bmp, map, player);
	ft_ray_cast(player, map, sdl_data->bmp);
	SDL_BlitSurface(sdl_data->bmp, NULL, sdl_data->display, NULL);
	// SDL_BlitSurface(textures[3].img, NULL, display, NULL);
	SDL_UpdateWindowSurface(sdl_data->win);
	// Refresh buffers
	SDL_FillRect(sdl_data->bmp, NULL, 0x000000);
	SDL_FillRect(sdl_data->display, NULL, 0x000000);
}

int	main()
{
	t_map		*map;
	t_player	player;
	
	t_sdl_data sdl_data;
	sdl_data.quit = false;
	sdl_data.win = ft_sdl_init_create_window(500, 400, WIN_WIDTH, WIN_HEIGHT);
	sdl_data.bmp = ft_create_surface(sdl_data.win, BMP_WIDTH, BMP_HEIGHT, BPP);
	sdl_data.display = SDL_GetWindowSurface(sdl_data.win);
	ft_create_player(&player, 125, 125, (t_vec3){ -1, 0, 0 });
	map = ft_create_map(10, 5);
	//Initialize PNG loading
	if(!(IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG) || !get_all_textures())
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		exit(1);
	}

	while (!sdl_data.quit)
	{
		while (SDL_PollEvent(&sdl_data.event))
		{
			// printf("\e[1;1H\e[2J");
			if (sdl_data.event.type == SDL_QUIT || sdl_data.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				sdl_data.quit = true;
			ft_player_controller(&player, sdl_data.event);
			// printf("playerPos: %f, %f\nscanCode %d | keyCode %d \n", player->pos.x, player->pos.y, scan_code_down(event), key_code(event));
		}
		ft_apply_physics(&player, map);
		// ft_print_map(map, player);
		ft_apply_render(&sdl_data, map, &player);
		
	}
	ft_destroy_map(map);
	SDL_FreeSurface(sdl_data.bmp);
	int k = -1;
	SDL_Surface **array_to_free = get_all_textures();
	while (++k < TEXTURE_NUM)
		SDL_FreeSurface(array_to_free[k]);
	SDL_DestroyWindow(sdl_data.win);
	SDL_Quit();
}
