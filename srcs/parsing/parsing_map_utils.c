/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:14:09 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/25 16:14:21 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	get_height(int *mapbeg, char **file)
{
	int		i;
	int		height;
	char	*trim;

	i = 0;
	height = 0;
	while (file && file[i])
	{
		trim = ft_strtrim(file[i], "\t ");
		if (!ft_strncmp(trim, "NO", 2) || !ft_strncmp(trim, "SO", 2)
			|| !ft_strncmp(trim, "WE", 2) || !ft_strncmp(trim, "EA", 2)
			|| !ft_strncmp(trim, "F", 1) || !ft_strncmp(trim, "C", 1)
			|| !ft_strncmp(trim, "\n", 1) || !trim[0])
		{
			i++;
			if (height == 0)
				(*mapbeg) = i;
			free(trim);
			continue ;
		}
		free(trim);
		height++;
		i++;
	}
	return (height);
}

int	check_char(char **file, int mapbeg)
{
	int	j;

	while (file && file[mapbeg])
	{
		j = 0;
		while (file[mapbeg][j])
		{
			if (file[mapbeg][j] != '1' && file[mapbeg][j] != '0'
				&& file[mapbeg][j] != '2'
				&& !ft_isspace(file[mapbeg][j]) && file[mapbeg][j] != 'N'
				&& file[mapbeg][j] != 'W' && file[mapbeg][j] != 'S'
				&& file[mapbeg][j] != 'E')
				return (error("Forbidden char in map", NULL));
			j++;
		}
		mapbeg++;
	}
	return (0);
}

int	get_max_len(char **file, int mapbeg)
{
	int	ret;
	int	i;

	ret = 0;
	while (file && file[mapbeg])
	{
		i = 0;
		while (file[mapbeg][i])
		{
			if (file[mapbeg][i] == '\t')
				i += 4;
			else
				i++;
		}
		if (i > ret)
			ret = i;
		mapbeg++;
	}
	return (ret);
}

int	parsing_allocation(t_map *parsing)
{
	int	i;

	i = 0;
	parsing->array = malloc(sizeof(char *) * (parsing->height + 1));
	if (! parsing->array)
		return (-1);
	while (i < parsing->height)
	{
		parsing->array[i] = malloc(sizeof(char) * (parsing->width + 1));
		if (! parsing->array[i])
		{
			while (--i >= 0)
				free(parsing->array[i]);
			free(parsing->array);
			return (-1);
		}
		i++;
	}
	parsing->array[i] = NULL;
	return (0);
}

void	replace_space(int i, t_map *parsing, char *line)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (j < parsing->width)
	{
		if (line[k] == '\t')
		{
			parsing->array[i][j] = ' ';
			parsing->array[i][++j] = ' ';
			parsing->array[i][++j] = ' ';
			parsing->array[i][++j] = ' ';
			k++;
		}
		else if (line[k] == '\0' )
			parsing->array[i][j] = ' ';
		else
		{
			parsing->array[i][j] = line[k];
			k++;
		}
		j++;
	}
	parsing->array[i][j] = '\0';
}
