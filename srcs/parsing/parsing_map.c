/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:49:07 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/29 15:50:25 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	fill_array(char **file, t_map *parsing, int mapbeg)
{
	int	j;

	j = 0;
	parsing->width = get_max_len(file, mapbeg);
	if (parsing_allocation(parsing))
		return (-1);
	while (file && file[mapbeg])
	{
		replace_space(j, parsing, file[mapbeg]);
		mapbeg++;
		j++;
	}
	replace_void(parsing);
	return (0);
}

int	check_open_wall(char **array, int y, int x)
{
	if (y < 0 || x < 0 || !array[y] || (array[y] && !array[y][x]))
		return (1);
	else if (array[y][x] == '1')
		return (0);
	array[y][x] = '1';
	return (check_open_wall(array, y - 1, x)
		+ check_open_wall(array, y + 1, x)
		+ check_open_wall(array, y, x - 1)
		+ check_open_wall(array, y, x + 1));
}

static int	get_player(t_vi2d *player, char **array)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[i][j])
		{
			if (array[i][j] == 'N' || array[i][j] == 'S'
				|| array[i][j] == 'W' || array[i][j] == 'E')
			{
				player->x = j;
				player->y = i;
				count++;
			}
			j++;
		}
		i++;
	}
	if (count != 1)
		return (-1);
	return (0);
}

int	loop_open_walls(char **dup)
{
	int	i;
	int	j;

	i = 0;
	while (dup[i])
	{
		j = 0;
		while (dup[i][j])
		{
			if (dup[i][j] == '0' && check_open_wall(dup, i, j))
			{
				free_split(dup);
				return (-1);
			}
			j++;
		}
		i++;
	}
	free_split(dup);
	return (0);
}

int	check_map(t_map *parsing, char **file)
{
	t_vi2d	player;
	char	**dup;
	int		mapbeg;

	mapbeg = 0;
	parsing->height = get_height(&mapbeg, file);
	if (check_char(file, mapbeg))
	{
		return (error("Forbidden char in map", NULL));
	}
	if (fill_array(file, parsing, mapbeg))
		return (-1);
	dup = dup_map(parsing->array, parsing->height);
	if (!dup)
		return (-1);
	if (get_player(&player, dup))
	{
		free_split(dup);
		return (error("Player issue in the file", NULL));
	}
	if (loop_open_walls(dup))
		return (error("Open walls", NULL));
	return (0);
}
