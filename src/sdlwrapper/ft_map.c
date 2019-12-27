#include "wolf3d.h"

void ft_print_map(t_map *map, t_player *player)
{
	int dirx = player->pos.x + player->forw.x;
	int diry = player->pos.y + player->forw.y;
	int posx = player->pos.x;
	int posy = player->pos.y;
	int k, l;
	char wall[map->width * 4];
	int i = -1;
	while (++i < (int)sizeof(wall) - 1)
		wall[i] = '-';

	k = -1;
	while (++k < map->height)
	{
		l = -1;
		while (++l < map->width)
		{
			if (map->tiles[k][l].depth > 0)
				printf(" %.f |", map->tiles[k][l].depth);
			else if ((posx >= l * map->tiles[k][l].width && posx < (l + 1) * map->tiles[k][l + 1].width)
					&& (posy >= k * map->tiles[k][l].height && posy < (k + 1) * map->tiles[k + 1][l].height))
				printf(" 8 |");
			else if ((dirx) == l && (diry) == k)
			{
				if ((dirx) > posx && (diry) == posy)
				printf(" > |");
				else if ((dirx) < posx && (diry) == posy)
				printf(" < |");
				else if ((dirx) == posx && (diry) < posy)
				printf(" ^ |");
				else if ((dirx) == posx && (diry) > posy)
				printf(" v |");
				else if ((dirx) > posx && (diry) > posy)
				printf(" * |");
				else if ((dirx) < posx && (diry) < posy)
				printf(" * |");
				else if ((dirx) > posx && (diry) < posy)
				printf(" * |");
				else if ((dirx) < posx && (diry) > posy)
				printf(" * |");
			}
			else
				printf("   |");
		}
		
		printf("\n%s\n", wall);
	}
}

void ft_fill_mini_map_wall(int *x_y, SDL_Surface *surface, t_map *map, int color)
{
	int tmp;
	int tmp1;

	tmp1 = x_y[1];
	while (tmp1 < x_y[1] + MINI_MAP_TILE_HEIGHT)
	{
		tmp = x_y[0];
		while (tmp < MINI_MAP_TILE_WIDTH + x_y[0])
		{
			 put_pixel32(surface, tmp++, tmp1, color);
		}
		tmp1++;
	}
	(void)map;
	(void)surface;
}
void ft_draw_mini_map(SDL_Surface *bmp, t_map *map, t_player *player)
{
	int k_l[2];
	int i_j[2];

	k_l[0] = -1;
	while (++k_l[0] < map->width)
	{
		k_l[1] = -1;
		while (++k_l[1] < map->height)
		{
			i_j[0] = k_l[0] * MINI_MAP_TILE_WIDTH;
			i_j[1] = k_l[1] * MINI_MAP_TILE_HEIGHT;
			if (map->tiles[k_l[1]][k_l[0]].depth)
				ft_fill_mini_map_wall(i_j, bmp, map, 0x99FFFFFF);
			else
				ft_fill_mini_map_wall(i_j, bmp, map, 0x990000FF);
		}
	}
	ft_draw_player(bmp, player->pos.x / MINI_MAP_RATIO_WIDTH, player->pos.y / MINI_MAP_RATIO_HEIGHT);
	(void)player;
}

void	ft_destroy_map(t_map *map)
{
	int i;

	i = -1;
	while (++i < map->height)
	{
		free(map->tiles[i]);
		map->tiles[i] = NULL;
	}
	free(map->tiles);
	map->tiles = NULL;
	free(map);
	map = NULL;
}

t_map	*ft_create_map(char *file)
{
	t_map *map;
	// int i;
	// int j;

	if (!(map = ft_get_map_from_file(file)))
		return (NULL);
	return (map);
	// exit(1);
	// if (!(map->tiles = (t_tile **)malloc(sizeof(t_tile *) * height)))
	// {
	// 	free(map);
	// 	return (NULL);
	// }
	// map->height = height;
	// map->width = width;
	// i = -1;
	// while (++i < height)
	// {
	// 	j = -1;
	// 	map->tiles[i] = (t_tile *)malloc(sizeof(t_tile) * width);
	// 	while (++j < width)
	// 	{
	// 		map->tiles[i][j].width = TILE_WIDTH;
	// 		map->tiles[i][j].height = TILE_HEIGHT;
	// 		map->tiles[i][j].index.x = j;
	// 		map->tiles[i][j].index.y = i;
	// 		// if (i == 0 || j == 0 || i == height - 1)
	// 		// 	map->tiles[i][j].depth = 1;
	// 		// else
	// 		// 	map->tiles[i][j].depth = 0;
	// 		// map->tiles[i][j].pos = (t_vec3int){ i, j, 0 };
	// 	}
	// }
	// map->tiles[1][4].depth = 1;
	// map->tiles[3][3].depth = 1;
	// map->tiles[2][3].depth = 1;
	// map->tiles[0][0].depth = 1;
	// return (map);
}

// t_map	*ft_create_map(int width, int height)
// {
// 	t_map *map;
// 	int i;
// 	int j;

// 	if (height < 4 || width < 4)
// 		return (NULL);
// 	if (!(map = (t_map *)malloc(sizeof(t_map))))
// 		return (NULL);
// 	if (!(map->tiles = (t_tile **)malloc(sizeof(t_tile *) * height)))
// 	{
// 		free(map);
// 		return (NULL);
// 	}
// 	map->height = height;
// 	map->width = width;
// 	i = -1;
// 	while (++i < height)
// 	{
// 		j = -1;
// 		map->tiles[i] = (t_tile *)malloc(sizeof(t_tile) * width);
// 		while (++j < width)
// 		{
// 			map->tiles[i][j].width = TILE_WIDTH;
// 			map->tiles[i][j].height = TILE_HEIGHT;
// 			map->tiles[i][j].index.x = j;
// 			map->tiles[i][j].index.y = i;
// 			// if (i == 0 || j == 0 || i == height - 1)
// 			// 	map->tiles[i][j].depth = 1;
// 			// else
// 			// 	map->tiles[i][j].depth = 0;
// 			// map->tiles[i][j].pos = (t_vec3int){ i, j, 0 };
// 		}
// 	}
// 	map->tiles[1][4].depth = 1;
// 	map->tiles[3][3].depth = 1;
// 	map->tiles[2][3].depth = 1;
// 	map->tiles[0][0].depth = 1;
// 	return (map);
// }

void ft_draw_mini_map_wall_inter(t_ray_hit wall, SDL_Surface *bmp)
{
	put_pixel32(bmp, wall.point.x / MINI_MAP_RATIO_WIDTH, wall.point.y / MINI_MAP_RATIO_HEIGHT, 0xFFFFFFFF);
}