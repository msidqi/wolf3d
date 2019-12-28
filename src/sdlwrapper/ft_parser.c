/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/27 09:28:54 by msidqi            #+#    #+#             */
/*   Updated: 2019/12/27 09:28:57 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void    ft_put_error(char *traceback)
{
	ft_putstr_fd("traceback (", 2);
	ft_putstr_fd(traceback, 2);
	ft_putstr_fd(") | Error: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

// char *ft_read_line(char *file)
// {
// }

int ft_read_map_size(int fd, t_map *map)
{
	int     ret;
	char    *line;
	char    *found;
	int     rvalue;

	rvalue = get_next_line(fd, &line);
	if (rvalue <= 0)
	{
		ret = 0;
	}
	else
	{
		map->height = ft_atoi(line);
		map->width = (found = ft_strchr(line, ' ')) == NULL ? 0 : ft_atoi(found);
		ret = map->width == 0 ? 0 : 1;
		free(line);
	}
	return (ret);
}

static void	ft_free_prev_tiles(int i, t_map *map)
{
	while (i >= 0)
	{
		ft_memdel((void **)&map->tiles[i]);
		i--;
	}
}

static void	ft_clean_tiles(int i, char *error, t_map *map)
{
	ft_free_prev_tiles(i, map);
	ft_memdel((void **)&map->tiles);
	perror(error);
}

int		ft_fill_map_tiles(int fd, t_map *map)
{
	char    *line;
	int     rvalue;
	int		i;
	int		j;

	i = 0;
	while ((rvalue = get_next_line(fd, &line)))
	{// printf("%zu, w %d h %d | %s\n", ft_strlen(line), map->width, map->height, line);
		if (i >= map->height || ft_strlen(line) != (size_t)map->width || !(map->tiles[i] = (t_tile *)malloc(sizeof(t_tile) * map->width)))
		{
			ft_clean_tiles(i - 1 , "Invalid number of tiles.\n", map);
			return (0);
		}
		j = -1;
		while (line[++j])
		{
			if (line[j] != '0' && line[j] != '1')
			{
				ft_clean_tiles(i, "map should contain only 1's and 0's.\n", map);
				return (0);
			}
			map->tiles[i][j].width = TILE_WIDTH;
			map->tiles[i][j].height = TILE_HEIGHT;
			map->tiles[i][j].index.x = j;
			map->tiles[i][j].index.y = i;
			map->tiles[i][j].depth = line[j] == '1' ? 1.0 : 0.0;// ft_putchar(line[j]);
		}
		free(line);
		i++;
		if (i == map->height)
			break ;
	}
	return (1);
}

t_map	*ft_get_map_from_file(int fd)
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
	printf("height %d width %d\n", map->height, map->width);
	if (!(map->tiles = (t_tile **)malloc(sizeof(t_tile *) * map->height))
		|| !(ft_fill_map_tiles(fd, map)))
	{
		printf("Error in ft_fill_map_tiles\n");
		ft_memdel((void **)&map);
		return (NULL);
	}
	close(fd);
	return (map);
}
