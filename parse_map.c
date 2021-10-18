/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:50:36 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/18 10:14:48 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	ft_fill_array(char *filename, t_map *m)
{
	int		fd;
	int		ret;
	int		j;

	ret = 1;
	j = 0;
	fd = open(filename, O_RDONLY);
	m->array = malloc(sizeof(char *) * (m->height + 1));
	if (!m->array)
		return (0);
	while (ret && ret != -1)
	{
		ret = get_next_line(fd, &m->array[j]);
		j++;
		if (ret == 0)
			break ;
	}
	m->array[j] = NULL;
	close(fd);
	return (1);
}

static size_t	ft_get_width(char *filename)
{
	int		fd;
	char	*line;
	size_t	len;
	
	fd = open(filename, O_RDONLY);
	get_next_line(fd, &line);
	if (!*line)
		ft_return_error("invalid map (empty)", 0);
	len = ft_strlen(line);
	while (get_next_line(fd, &line))
		free(line);
	free(line);
	close(fd);
	return (len);
}

static size_t	ft_get_height(char *filename)
{
	int		fd;
	char	*line;
	size_t	count;

	count = 1;
	fd = open(filename, O_RDONLY);
	if (!fd)
		return (0);
	while (get_next_line(fd, &line) == 1)
	{
		count++;
		free(line);
	}
	free(line);
	close(fd);
	return (count);
}

int	ft_check_valid(char *filename, t_map *map)
{
	map->height = ft_get_height(filename);
	map->width = ft_get_width(filename);
	if (map->height > 0 && map->width > 0)
	{
		return (ft_fill_array(filename, map));
	}
	else
		ft_putendl_fd("map error", 2);
	return (0);
}