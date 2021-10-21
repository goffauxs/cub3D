/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 10:07:59 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/21 15:08:21 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	parsing_init(t_map *parsing)
{
	int i;

	i = -1;
	while (++i < 4)
		parsing->tex_path[i] = NULL;
	parsing->array = NULL;
}

void	parsing_free(t_map *parsing)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (parsing->tex_path[i])
			free(parsing->tex_path[i]);
		i++;
	}
	if (parsing->array)
		free_split(parsing->array);
}

int	cub_check(int argc, char **argv, t_map *parsing)
{
	char	**file;

	file = NULL;
	if (check_args(argc, argv))
		return (-1);
	file = dup_cub(argv, file);
	if (!file)
		return (-1);
	if (check_textures(parsing, file))
		return (return_split_free(file));
	if (check_floor(parsing, file))
		return (return_split_free(file));
	if (check_map(parsing, file))
		return (return_split_free(file));
	free_split(file);
	return (0);
}
