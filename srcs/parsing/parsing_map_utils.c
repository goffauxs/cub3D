/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:14:09 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/29 15:50:44 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	get_height(int *mapbeg, char **file)
{
	int		i;
	char	*trim;

	i = 0;
	while (file && file[i])
	{
		trim = ft_strtrim(file[i], "\t ");
		if (!ft_strncmp(trim, "NO ", 3) || !ft_strncmp(trim, "SO ", 3)
			|| !ft_strncmp(trim, "WE ", 3) || !ft_strncmp(trim, "EA ", 3)
			|| !ft_strncmp(trim, "F ", 2) || !ft_strncmp(trim, "C ", 2)
			|| !ft_strncmp(trim, "\n", 1) || !trim[0])
			(*mapbeg) = i;
		else
		{
			free(trim);
			break ;
		}
		free(trim);
		i++;
	}
	i = ++(*mapbeg);
	while (file && file[i])
		i++;
	return (i - *mapbeg);
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
				return (-1);
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
	int	j;

	ret = 0;
	while (file && file[mapbeg])
	{
		i = 0;
		j = 0;
		while (file[mapbeg][j])
		{
			if (file[mapbeg][j] == '\t')
				i += 4;
			else
				i++;
			j++;
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
		else if (line[k] == '\0')
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
