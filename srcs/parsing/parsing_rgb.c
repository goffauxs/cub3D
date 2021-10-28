/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rgb.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 16:23:38 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/28 17:19:32 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
		if (!ft_strncmp(trim, "F ", 2))
		{
			if (check_rgb(&count, trim, parsing, "F \t"))
				return (error("Wrong RGB formatting", trim));
		}
		if (!ft_strncmp(trim, "C ", 2))
		{
			if (check_rgb(&count, trim, parsing, "C \t"))
				return (error("Wrong RGB formatting", trim));
		}
		free(trim);
		i++;
	}
	if (count != 2)
		return (error("Wrong number of colors", NULL));
	return (0);
}
