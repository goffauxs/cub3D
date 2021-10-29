/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:17:59 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/29 15:44:25 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	replace_void(t_map *parsing)
{
	int		i;
	int		j;
	char	**dup;

	i = 0;
	while (parsing->array[i])
	{
		j = 0;
		while (parsing->array[i][j])
		{
			if (parsing->array[i][j] == ' ')
			{
				dup = dup_map(parsing->array, parsing->height);
				if (!check_open_wall(dup, i, j))
					parsing->array[i][j] = '0';
				free_split(dup);
			}
			j++;
		}
		i++;
	}
}
