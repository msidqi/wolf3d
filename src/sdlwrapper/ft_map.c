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

void ft_fill_wall(int x, int y, SDL_Surface *surface, t_map *map)
{
	int tmp;
	int tmp1;

	tmp1 = y;
	while (tmp1 < y + MINI_MAP_TILE_HEIGHT)
	{
		tmp = x;
		while (tmp < MINI_MAP_TILE_WIDTH + x)
		{
			 put_pixel32(surface, tmp++, tmp1, 0x99FFFFFF);
		}
		tmp1++;
	}
	(void)map;
	(void)surface;
}
void ft_draw_map(SDL_Surface *surface, t_map *map, t_player *player)
{
	int k, l;
	k = -1;
	while (++k < map->width)
	{
		l = -1;
		while (++l < map->height)
		{
			if (map->tiles[l][k].depth)
				ft_fill_wall(k * MINI_MAP_TILE_WIDTH, l * MINI_MAP_TILE_HEIGHT, surface, map);
		}
	}
	l = -1;
	while (++l <= map->width)
	{
		k = -1;
		while (++k < map->height * MINI_MAP_TILE_HEIGHT)
		{
			put_pixel32(surface, l * MINI_MAP_TILE_WIDTH, k, 0xFFFFFFFF);
		}
	}
	k = -1;
	while (++k <= map->height)
	{
		l = -1;
		while (++l < map->width * MINI_MAP_TILE_WIDTH)
		{
			put_pixel32(surface, l, k * MINI_MAP_TILE_HEIGHT, 0xFFFFFFFF);
		}
	}
	put_pixel32(surface, player->pos.x / MINI_MAP_RATIO_WIDTH, player->pos.y / MINI_MAP_RATIO_HEIGHT, 0xFF00FFFF);
	put_pixel32(surface, player->pos.x / MINI_MAP_RATIO_WIDTH + 1, player->pos.y / MINI_MAP_RATIO_HEIGHT, 0xFF00FFFF);
	put_pixel32(surface, player->pos.x / MINI_MAP_RATIO_WIDTH, player->pos.y / MINI_MAP_RATIO_HEIGHT + 1, 0xFF00FFFF);
	put_pixel32(surface, player->pos.x / MINI_MAP_RATIO_WIDTH - 1, player->pos.y / MINI_MAP_RATIO_HEIGHT, 0xFF00FFFF);
	put_pixel32(surface, player->pos.x / MINI_MAP_RATIO_WIDTH, player->pos.y / MINI_MAP_RATIO_HEIGHT - 1, 0xFF00FFFF);
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

t_map	*ft_create_map(int width, int height)
{
	t_map *map;
	int i;
	int j;

	if (height < 4 || width < 4)
		return (NULL);
	map = (t_map *)malloc(sizeof(t_map));


	map->tiles = (t_tile **)malloc(sizeof(t_tile *) * height);
	map->height = height;
	map->width = width;
	i = -1;
	while (++i < height)
	{
		j = -1;
		map->tiles[i] = (t_tile *)malloc(sizeof(t_tile) * width);
		while (++j < width)
		{
			map->tiles[i][j].width = TILE_WIDTH;
			map->tiles[i][j].height = TILE_HEIGHT;
			map->tiles[i][j].index.x = j;
			map->tiles[i][j].index.y = i;
			if (i == 0 || j == 0 || i == height - 1)
				map->tiles[i][j].depth = 1;
			else
				map->tiles[i][j].depth = 0;
			map->tiles[i][j].pos = (t_vec3int){ i, j, 0 };
		}
	}
	map->tiles[1][4].depth = 1;
	map->tiles[3][3].depth = 1;
	map->tiles[2][3].depth = 1;
	map->tiles[0][0].depth = 1;
	return (map);
}
