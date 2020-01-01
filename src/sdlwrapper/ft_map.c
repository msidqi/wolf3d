/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msidqi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/01 11:12:20 by msidqi            #+#    #+#             */
/*   Updated: 2020/01/01 11:12:25 by msidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_destroy_map(t_map *map)
{
	int i;

	i = -1;
	if (map == NULL)
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
	t_map	*map;
	int		fd;

	if ((fd = open(file, O_RDONLY)) == -1)
	{
		ft_put_error("ft_read_map_file");
		return (NULL);
	}
	if (!(map = ft_get_map_from_file(fd, player)))
		return (NULL);
	return (map);
}
