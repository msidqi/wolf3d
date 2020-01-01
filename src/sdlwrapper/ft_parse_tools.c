/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 16:36:13 by aabouibr          #+#    #+#             */
/*   Updated: 2019/12/31 16:38:54 by aabouibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	ft_put_error(char *traceback)
{
	ft_putstr_fd("traceback (", 2);
	ft_putstr_fd(traceback, 2);
	ft_putstr_fd(") | Error: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

int		ft_read_map_size(int fd, t_map *map)
{
	int		ret;
	char	*line;
	char	*found;
	int		rvalue;

	rvalue = get_next_line(fd, &line);
	if (rvalue <= 0)
	{
		ret = 0;
	}
	else
	{
		map->height = ft_atoi(line);
		found = ft_strchr(line, ' ');
		map->width = (found) == NULL ? 0 : ft_atoi(found);
		ret = map->width == 0 ? 0 : 1;
		free(line);
	}
	return (ret);
}

void	ft_free_prev_tiles(int i, t_map *map)
{
	while (i >= 0)
	{
		ft_memdel((void **)&map->tiles[i]);
		i--;
	}
}

void	ft_clean_tiles(int i, char *error, t_map *map)
{
	ft_free_prev_tiles(i, map);
	ft_memdel((void **)&map->tiles);
	perror(error);
}
