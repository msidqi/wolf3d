/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/27 09:28:54 by msidqi            #+#    #+#             */
/*   Updated: 2019/12/31 19:17:04 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		ft_fill_tile(int i, int j, char c, t_tile *tile)
{
	tile->width = TILE_WIDTH;
	tile->height = TILE_HEIGHT;
	tile->index.x = j;
	tile->index.y = i;
	tile->depth = (c == '0' || c == 'P') ? 0.0 : 1.0;
}

void		ft_fill_pos(int i, int j, t_vec3 *pos)
{
	pos->x = j * TILE_WIDTH + 0.5;
	pos->y = i * TILE_HEIGHT + 0.5;
	pos->z = 0;
}

int			ft_parse_line(char *line, t_player *player, t_map *map, int i)
{
	int		j;

	j = -1;
	while (line[++j])
	{
		if (line[j] == '0' || line[j] == '1' || line[j] == 'P')
		{
			ft_fill_tile(i, j, line[j], &map->tiles[i][j]);
			if (line[j] == 'P')
				ft_fill_pos(i, j, &player->pos);
		}
		else
		{
			ft_clean_tiles(i, "map should contain only (0|1|P)'s.\n", map);
			free(line);
			return (0);
		}
	}
	return (1);
}

int			ft_fill_map_tiles(int fd, t_map *map, t_player *player)
{
	char	*line;
	int		i;

	i = 0;
	while (get_next_line(fd, &line) && i != map->height)
	{
		if (i == 72 || ft_strlen(line) > 128
			|| ft_strlen(line) != (size_t)map->width
			|| !(map->tiles[i] = (t_tile *)malloc(sizeof(t_tile) * map->width)))
		{
			ft_clean_tiles(i - 1, "Invalid number of tiles.\n", map);
			return (0);
		}
		if (!(ft_parse_line(line, player, map, i)))
			return (0);
		free(line);
		i++;
	}
	if (i != map->height)
	{
		ft_clean_tiles(i - 1, "Invalid number of tiles.\n", map);
		return (0);
	}
	return (1);
}

t_map		*ft_get_map_from_file(int fd, t_player *player)
{
	t_map *map;

	if (!(map = (t_map *)malloc(sizeof(t_map))))
		return (NULL);
	if (!ft_read_map_size(fd, map) || map->height < 4 || map->width < 4)
	{
		perror("Error: map size too small\n");
		ft_memdel((void **)&map);
		return (NULL);
	}
	if (!(map->tiles = (t_tile **)malloc(sizeof(t_tile *) * map->height))
		|| !(ft_fill_map_tiles(fd, map, player)))
	{
		perror("Error in ft_fill_map_tiles\n");
		ft_memdel((void **)&map);
		return (NULL);
	}
	close(fd);
	return (map);
}
