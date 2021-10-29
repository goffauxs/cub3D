/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:59 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/29 14:16:13 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

char	**dup_bis(int len, char **argv, char **ret)
{
	int		fd;
	int		i;
	int		v;
	char	*line;

	ret = malloc(sizeof(char *) * (len + 1));
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
	return (dup_bis(i, argv, ret));
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
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(msg, 2);
	return (-1);
}

char	**dup_map(char **array, int height)
{
	char	**tmp;
	int		z;

	tmp = malloc(sizeof(char *) * (height + 1));
	if (!tmp)
		return (NULL);
	z = 0;
	while (z < height)
	{
		tmp[z] = ft_strdup(array[z]);
		z++;
	}
	tmp[z] = NULL;
	return (tmp);
}
