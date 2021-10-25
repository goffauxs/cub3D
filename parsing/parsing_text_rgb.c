/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_text_rgb.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:49:07 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/25 12:21:00 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	check_args(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
		return (error("Invalid number of arguments", NULL));
	if (ft_strncmp(".cub", &argv[1][ft_strlen(argv[1]) - 4], 4))
		return (error("Wrong map format", NULL));
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		close(fd);
		return (error("Does this map exist ?", NULL));
	}
	close(fd);
	return (0);
}

int	check_textures(t_map *parsing, char **file)
{
	int		i;
	char	*trim;

	i = 0;
	while (file && file[i])
	{
		trim = ft_strtrim(file[i], " \t");
		if (!ft_strncmp(trim, "NO", 2))
			parsing->tex_path[NORTH] = ft_strtrim(trim, "NO \t");
		else if (!ft_strncmp(trim, "SO", 2))
			parsing->tex_path[SOUTH] = ft_strtrim(trim, "SO \t");
		else if (!ft_strncmp(trim, "WE", 2))
			parsing->tex_path[WEST] = ft_strtrim(trim, "WE \t");
		else if (!ft_strncmp(trim, "EA", 2))
			parsing->tex_path[EAST] = ft_strtrim(trim, "EA \t");
		free(trim);
		i++;
	}
	if (!parsing->tex_path[NORTH] || !parsing->tex_path[SOUTH]
		|| !parsing->tex_path[WEST] || !parsing->tex_path[EAST])
		return (error("Error: textures missing", NULL));
	if (open(parsing->tex_path[NORTH], O_RDONLY) == -1
		|| open(parsing->tex_path[SOUTH], O_RDONLY) == -1
		|| open(parsing->tex_path[WEST], O_RDONLY) == -1
		|| open(parsing->tex_path[EAST], O_RDONLY) == -1)
		return (error("Error: wrong texture path", NULL));
	return (0);
}

static int	check_digits(char **split)
{
	int		j;
	int		i;

	i = 0;
	while (split[i])
	{
		if (ft_atoi(split[i]) < 0 || ft_atoi(split[i]) > 255)
			return (-1);
		j = 0;
		while (split[i][j])
		{
			if (!ft_isdigit(split[i][j]) && !ft_isspace(split[i][j]))
				return (-1);
			j++;
		}
		i++;
	}
	if (i != 3)
		return (-1);
	return (0);
}

static	int	check_rgb(int *count, char *line, t_map *parsing, char *str)
{
	char	**split;
	char	*trim;
	int		i;

	i = 0;
	trim = ft_strtrim(line, str);
	split = ft_split(trim, ',');
	free(trim);
	if (check_digits(split))
	{
		free_split(split);
		return (-1);
	}
	while (split[i])
	{
		if (str[0] == 'F')
			parsing->floor[i] = ft_atoi(split[i]);
		else
			parsing->ceil[i] = ft_atoi(split[i]);
		i++;
	}
	(*count)++;
	free_split(split);
	return (0);
}

int	check_floor(t_map *parsing, char **file)
{
	char	*trim;
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (file && file[i])
	{
		trim = ft_strtrim(file[i], " \t");
		if (!ft_strncmp(trim, "F", 1))
		{
			if (check_rgb(&count, trim, parsing, "F \t"))
				return (error("Error: wrong RGB formatting", trim));
		}
		if (!ft_strncmp(trim, "C", 1))
		{
			if (check_rgb(&count, trim, parsing, "C \t"))
				return (error("Error: wrong RGB formatting", trim));
		}
		free(trim);
		i++;
	}
	if (count != 2)
		return (error("Error: wrong number of colors", NULL));
	return (0);
}
