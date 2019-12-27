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
	return ((t_vec3){ -2, -2, -2});
}

void	ft_find_vertical_intersection(t_ray *ray, t_map *map)
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

void	ft_find_horizontal_intersection(t_ray *ray, t_map *map)
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

void ft_find_closest_wall(t_ray *ray, t_map *map, t_vec3 forward)//t_player *player)
{
	t_tile *tile = &map->tiles[0][0];
	t_tile *tile2 = &map->tiles[0][0];
	t_vec3 i = ray->first_hor_point;
	t_vec3 j = ray->first_ver_point;
	double cos_alpha;
	t_inter	hor;
	t_inter	ver;

	hor.intersected = 0;
	ver.intersected = 0;
	hor.distance = MEGA;
	ver.distance = MEGA;
	ray->ray_hit.distance_from_origin = MEGA;
	while ((tile || tile2) && (!hor.intersected || !ver.intersected))
	{
		if (!hor.intersected && (tile = get_map_tile_hor(get_map_index(i, map), map)))
		{
			if (tile->depth == 1)
			{
				cos_alpha = ft_vec3_dot_product(forward, ray->dir);
				hor.distance = ft_vec3_mag(ft_vec3_sub(i, ray->origin)) * cos_alpha;
				hor.facing = ((int)(i.y / TILE_HEIGHT) == tile->index.y) ? NORTH : SOUTH;
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
				cos_alpha = ft_vec3_dot_product(forward, ray->dir);
				ver.distance = ft_vec3_mag(ft_vec3_sub(j, ray->origin))  * cos_alpha;
				ver.facing = ((int)(j.x / TILE_WIDTH) == tile2->index.x) ? WEST : EAST;
				ver.point = j;
				ver.intersected = 1;
				tile2 = NULL;
			}
			j = ft_vec3_add(j, ray->increments_v);
		}
	}
	ray->ray_hit.facing = SKYBOX;
	if (hor.distance < ver.distance)
	{
		ray->ray_hit.point = hor.point;
		ray->ray_hit.distance_from_origin = hor.distance;
		ray->ray_hit.facing = hor.facing;
		ray->ray_hit.type = WALL;
	}
	else if (ver.distance >= ver.distance && ver.distance != MEGA)
	{
		ray->ray_hit.point = ver.point;
		ray->ray_hit.distance_from_origin = ver.distance;
		ray->ray_hit.facing = ver.facing;
		ray->ray_hit.type = WALL;
	}
}

/*
**	initializes textures on first call only
**	returns an array of SDL surfaces whenever called
*/

SDL_Surface **get_all_textures(void)
{
	static int initialized = 0;
	static SDL_Surface *game_textures[TEXTURE_NUM];
	int i;

	if (!initialized)
	{
		game_textures[0] = IMG_Load("./Textures/NORTH.JPG");
		game_textures[1] = IMG_Load("./Textures/SOUTH.JPG");
		game_textures[2] = IMG_Load("./Textures/EAST.JPG");
		game_textures[3] = IMG_Load("./Textures/WEST.JPG");
		// IMG_Load("SKY.JPG"),
		// IMG_Load("GROUND.JPG"),
		i = -1;
		while (++i < TEXTURE_NUM)
			if (!game_textures[i])
				return (NULL);
		initialized = 1;

	}
	return (game_textures);
}

int get_wall_texture(int y, t_ray_hit wall, SDL_Surface *bmp)
{	
	SDL_Surface **textures;
	int color;
	textures = get_all_textures();
	int ny = (textures[wall.facing]->h) * (y - (bmp->h / 2 - wall.wall_height / 2)) / wall.wall_height;
	int nx;
	if (wall.facing == NORTH || wall.facing == SOUTH)
	{
		nx = (textures[wall.facing]->w) * (ceil(wall.wall_height - wall.point.x) - (wall.wall_height - wall.point.x));
		color = getpixel(textures[wall.facing], nx, (ny));
	}
	else if (wall.facing == EAST || wall.facing == WEST)
	{
		nx = (textures[wall.facing]->w) * (ceil(wall.wall_height - wall.point.y) - (wall.wall_height - wall.point.y));
		color = getpixel(textures[wall.facing], nx, (ny));
	}
	if (wall.facing == SKYBOX)
		return (0xFFFFFFFF);
	return (0xFF000000 + color);
}
void ft_draw_walls(int x, t_ray_hit wall, SDL_Surface *bmp, t_player *player)
{
	int i;
	int j;
	int counter;
	if (wall.facing == SKYBOX)
		return ;// draw skybox
	i = bmp->h / 2 + player->height;
	j = i + 1;
	counter = 0;
	wall.wall_height = (int)(bmp->h / wall.distance_from_origin + 20);
	while (counter < wall.wall_height / 2)
	{
		if (i >= 0)
		{
			put_pixel32(bmp, x, i, get_wall_texture(i - player->height, wall, bmp));
			i--;
		}
		if (j + player->height < bmp->h)
		{
			put_pixel32(bmp, x, j, get_wall_texture(j - player->height, wall, bmp));
			j++;
		}
		counter++;
	}

}

int ft_ray_cast(t_ray *ray, t_vec3 origin, t_vec3 direction, t_map *map)
{
	ray->dir = direction;
	ray->origin = origin;
	ft_find_vertical_intersection(ray, map);
	ft_find_horizontal_intersection(ray, map);
	return (1);
}

/* void ft_draw_walls(int x, t_ray_hit wall, SDL_Surface *bmp, t_player *player)
{
	int i;
	int j;
	int counter;
	double wall_height;

	if (wall.facing == SKYBOX)
		return ;// draw skybox
	i = bmp->h / 2 + player->height;
	j = i + 1;
	counter = 0;
	wall_height = (int)(bmp->h / wall.distance_from_origin + 20);
	int txt0;
	int txt1;
	txt0 = wall_height / 2;
	txt1 = txt0 + 1;

	double mapped_x;
	if (wall.facing == NORTH || wall.facing == SOUTH)
		mapped_x = (int)wall.point.x % TILE_WIDTH / TILE_WIDTH;
	else
		mapped_x = (int)wall.point.y % TILE_HEIGHT / TILE_HEIGHT;
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
		if (j + player->height < bmp->h && txt1 < wall_height)
		{
			put_pixel32(bmp, x, j, get_wall_texture(mapped_x, txt1 / wall_height, wall, bmp));
			txt1++;
			j++;
		}
		counter++;
	}
	(void)x;
}*/

void	ft_ray_cast_scene(t_player *player, t_map *map, SDL_Surface *bmp)
{
	int x;
	t_vec3	mapped_pos;
	t_ray ray;
	t_vec3 direction;

	x = -1;
	while (++x < BMP_WIDTH)
	{
		mapped_pos = ft_map_pixels_to_world(x, player);
		direction = ft_vec3_normalize(ft_vec3_sub(mapped_pos, player->pos));
		ft_ray_cast(&ray, player->pos, direction, map);

		ft_find_closest_wall(&ray, map, player->forw);
		if (ray.ray_hit.type == WALL)
		{
			ft_draw_walls(x, ray.ray_hit, bmp, player);
			ft_draw_mini_map_wall_inter(ray.ray_hit, bmp);
		}
		if (ray.ray_hit.type == PROJECTILE)
		{
			
		}
	}
}

void ft_fill_background(SDL_Surface *bmp, t_player *player)
{
	(void)player;
	int x;
	int y;
	// double perc;

	x = 0;
	while (x < bmp->w)
	{
		y = 0;
		while (y < bmp->h / 2 + player->height)
		{
			// perc = ((double)y / (double)(bmp->h / 2));
			// ft_color_rgb_scalar(0xFF5381FF, perc, perc, perc)
			put_pixel32(bmp, x, y, 0xFF393739);
			y++;
		}
		x++;
	}
	x = 0;
	while (x < bmp->w)
	{
		y = bmp->h / 2 + player->height;
		while (y < bmp->h)
		{
			// perc = ((double)y / (double)(bmp->h / 2));
			 put_pixel32(bmp, x, y, 0xFF717171);
			 y++;
		}
		x++;
	}
}

void	ft_apply_physics(t_player *player, t_map *map)
{
	static Uint64 previous_tick = 0;
	// Uint64 delta_time;
	
	if (!previous_tick)
		previous_tick = SDL_GetPerformanceCounter();
	if (SDL_GetPerformanceCounter() - previous_tick > (Uint64)SECOND / 60)
	{
		// printf("PHYSICS\n");
		ft_player_physics(player, map);
		previous_tick = SDL_GetPerformanceCounter();
	}
	(void)map;
	(void)player;
}

void ft_fps_counter(void)
{
	static int prev_tick = 0;
	static int fps = 0;

	fps++;
	if (!prev_tick)
		prev_tick = SDL_GetTicks();
	if (((SDL_GetTicks() - prev_tick)) > 1000)
	{
		printf("\033[2J\nfps %d\n", fps);
		prev_tick = SDL_GetTicks();
		fps = 0;
	}
}

void	ft_debug_screen_line(SDL_Surface *bmp)
{
	int i;
	
	i = -1;
	while(++i < bmp->h)
	{
		put_pixel32(bmp, bmp->w / 2, i, 0xFFFFFFFF);
	}
}

void ft_clear_screen(t_sdl_data *sdl_data)
{
	// Refresh buffers
	SDL_FillRect(sdl_data->mini_map_bmp, NULL, 0x000000);
	SDL_FillRect(sdl_data->bmp, NULL, 0x000000);
	SDL_FillRect(sdl_data->display, NULL, 0x000000);
}

void	ft_update_screen(t_sdl_data *sdl_data)
{
	SDL_BlitSurface(sdl_data->bmp, NULL, sdl_data->display, NULL);
	SDL_BlitSurface(sdl_data->mini_map_bmp, NULL, sdl_data->display, NULL);
	SDL_UpdateWindowSurface(sdl_data->win);
}

void ft_apply_render(t_sdl_data *sdl_data, t_map *map, t_player *player)
{
	ft_clear_screen(sdl_data);
	// ft_fps_counter();
	ft_fill_background(sdl_data->bmp, player);
	ft_draw_mini_map(sdl_data->mini_map_bmp, map, player);
	ft_ray_cast_scene(player, map, sdl_data->bmp);
	// ft_debug_screen_line(sdl_data->bmp);
	ft_update_screen(sdl_data);
}

void	ft_free_textures(void)
{
	int k = -1;
	SDL_Surface **array_to_free;
	
	array_to_free = get_all_textures();
	while (++k < TEXTURE_NUM)
		if (array_to_free[k])
			SDL_FreeSurface(array_to_free[k]);
}

void	ft_free_surface(t_sdl_data *sdl_data)
{
	if (sdl_data->bmp)
		SDL_FreeSurface(sdl_data->bmp);
	if (sdl_data->mini_map_bmp)
		SDL_FreeSurface(sdl_data->mini_map_bmp);
}
void	ft_graceful_shutdown(t_sdl_data *sdl_data, t_map *map)
{
	ft_destroy_map(map);
	ft_free_textures();
	ft_free_surface(sdl_data);
	if (sdl_data->win != NULL)
		SDL_DestroyWindow(sdl_data->win);
	// Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(1);
}

int	main(void)
{
	t_map		*map;
	t_player	player;
	t_sdl_data  sdl_data;

	ft_sdl_init_data(&sdl_data);
	ft_create_player(&player, (int)(TILE_WIDTH + 1) , (int)(TILE_HEIGHT + 1) , (t_vec3){ -1, 0, 0 });
	if (!(map = ft_create_map("level1.map")))
		return (1);
	//Initialize PNG loading
	if(!(IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG))
	{
		perror(IMG_GetError());
		exit(1);
	}
	//Init Textures
	get_all_textures();
	while (!sdl_data.quit)
	{
		while (SDL_PollEvent(&sdl_data.event))
		{
			// printf("\e[1;1H\e[2J");
			if (sdl_data.event.type == SDL_QUIT || sdl_data.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				sdl_data.quit = true;
			ft_player_input(&player, sdl_data.event);
		}
		ft_apply_physics(&player, map);
		ft_apply_render(&sdl_data, map, &player);
	}
	printf("EXIT!\n");
	ft_graceful_shutdown(&sdl_data, map);
}

		// int k, l;
		// k = -1;
		// while (++k < map->height)
		// {
		// 	l = -1;
		// 	while (++l < map->width)
		// 	{
		// 		printf("%d (%d, %d)| (%.0f, %.0f)...", (int)map->tiles[k][l].depth, map->tiles[k][l].index.x, map->tiles[k][l].index.y, map->tiles[k][l].width, map->tiles[k][l].height);
		// 		// printf("%d ", (int)map->tiles[k][l].depth);
		// 	}
		// 	printf("\n");
		// }
		// exit(1);
