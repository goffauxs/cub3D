/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_text.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:49:07 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/29 14:35:46 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	check_args(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
		return (error("Usage : ./cub3D <path to map>", NULL));
	if (ft_strncmp(".cub", &argv[1][ft_strlen(argv[1]) - 4], 4))
		return (error("Wrong map format", NULL));
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror(argv[1]);
		return (-1);
	}
	close(fd);
	return (0);
}

int	error_textures(t_map *parsing, int j)
{
	int		i;
	int		fd;

	if (!parsing->tex_path[NORTH] || !parsing->tex_path[SOUTH]
		|| !parsing->tex_path[WEST] || !parsing->tex_path[EAST] || j != 4)
		return (error("Wrong number of textures", NULL));
	i = 0;
	while (i < 4)
	{
		fd = open(parsing->tex_path[i], O_RDONLY);
		if (fd == -1)
			return (error("Wrong texture path", NULL));
		close(fd);
		i++;
	}
	return (0);
}

int	check_textures(t_map *parsing, char **file)
{
	int		i;
	int		j;
	char	*trim;

	i = -1;
	j = 0;
	while (file && file[++i])
	{
		trim = ft_strtrim(file[i], " \t");
		if (!ft_strncmp(trim, "NO ", 3) || !ft_strncmp(trim, "SO", 2)
			|| !ft_strncmp(trim, "WE ", 3) || !ft_strncmp(trim, "EA", 2))
			j++;
		if (!ft_strncmp(trim, "NO ", 3) && !parsing->tex_path[NORTH])
			parsing->tex_path[NORTH] = ft_strtrim(trim, "NO \t");
		else if (!ft_strncmp(trim, "SO ", 3) && !parsing->tex_path[SOUTH])
			parsing->tex_path[SOUTH] = ft_strtrim(trim, "SO \t");
		else if (!ft_strncmp(trim, "WE ", 3) && !parsing->tex_path[WEST])
			parsing->tex_path[WEST] = ft_strtrim(trim, "WE \t");
		else if (!ft_strncmp(trim, "EA ", 3) && !parsing->tex_path[EAST])
			parsing->tex_path[EAST] = ft_strtrim(trim, "EA \t");
		free(trim);
	}
	if (error_textures(parsing, j))
		return (-1);
	return (0);
}
