/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:49:07 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/25 16:25:22 by mdeclerf         ###   ########.fr       */
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

int	check_texture_path(t_map *parsing)
{
	int	i;
	int	fd;

	i = 0;
	while (i < 4)
	{
		fd = open(parsing->tex_path[i], O_RDONLY);
		if (fd == -1)
			return (-1);
		close(fd);
		i++;
	}
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
	if (check_texture_path(parsing))
		return (error("Error: wrong texture path", NULL));
	return (0);
}
