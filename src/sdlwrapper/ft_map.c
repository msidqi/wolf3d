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
	if (map != NULL)
		return ;
	while (++i < map->height)
	{
		if (map->tiles[i] != NULL)
			ft_memdel((void **)&(map->tiles[i]));
	}
	if (map->tiles)
		ft_memdel((void **)&map->tiles);
	ft_memdel((void **)&map);
}

t_map	*ft_create_map(char *file, t_player *player)
{
	t_map *map;
	int     fd;

	if ((fd = open(file, O_RDONLY)) == -1)
	{
		ft_put_error("ft_read_map_file");
		return (NULL);
	}
	if (!(map = ft_get_map_from_file(fd, player)))
		return (NULL);
	return (map);
}

void ft_draw_mini_map_wall_inter(t_ray_hit wall, SDL_Surface *bmp)
{
	int x;
	int y;

	x = wall.point.x / MINI_MAP_RATIO_WIDTH;
	y = wall.point.y / MINI_MAP_RATIO_HEIGHT;
	if (x >= 0 && y >= 0 && x < bmp->w && y < bmp->h)
		put_pixel32(bmp, x, y, 0xFFFFFFFF);
}