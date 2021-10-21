/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:59 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/21 11:34:37 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	**dup_cub(char **argv, char **ret)
{
	int		fd;
	char	*line;
	int		i;
	int		v;

	fd = open(argv[1], O_RDONLY);
	i = 0;
	v = 2;
	while (v > 0)
	{
		v = get_next_line(fd, &line);
		free(line);
		i++;
	}
	close(fd);
	ret = malloc(sizeof(char *) * (i + 1));
	fd = open(argv[1], O_RDONLY);
	i = 0;
	v = 2;
	while (v > 0)
	{
		v = get_next_line(fd, &line);
		ret[i++] = ft_strdup(line);
		free(line);
	}
	ret[i] = NULL;
	close(fd);
	return (ret);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split && split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	error(char *msg, char *free_ptr)
{
	if (free_ptr)
		free(free_ptr);
	ft_putendl_fd(msg, 2);
	return (-1);
}

int	return_split_free(char **split)
{
	free_split(split);
	return (-1);
}
