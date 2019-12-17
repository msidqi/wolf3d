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

void ft_draw_map(SDL_Surface *surface, t_map *map, t_player *player)
{
	int k, l;

	l = -1;
	while (++l <= map->width)
	{
		k = -1;
		while (++k < map->height * 100)
		{
			put_pixel32(surface, l * 100, k, 0xFFFFFFFF);
		}
		// exit(1);
	}
	k = -1;
	while (++k <= map->height)
	{
		l = -1;
		while (++l < map->width * 100)
		{
			put_pixel32(surface, l, k * 100, 0xFFFFFFFF);
		}
	}
	(void)player;
	(void)surface;
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
			map->tiles[i][j].width = 100;
			map->tiles[i][j].height = 100;
			// if (j > 5)
			// 	map->tiles[i][j].depth = 1;
			// else
				map->tiles[i][j].depth = 0;
			map->tiles[i][j].pos = (t_vec3int){ i, j, 0 };
		}
	}
	map->tiles[3][4].depth = 1;
	return (map);
}
