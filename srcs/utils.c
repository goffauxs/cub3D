/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:04:15 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/29 15:53:19 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	ft_return_error(const char *err_msg, int systeme)
{
	if (!systeme)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	else
		perror(err_msg);
	exit(-1);
}

double	ft_abs(double a)
{
	if (a < 0.0)
		return (-a);
	return (a);
}

double	bound_angle(double angle)
{
	if (angle < 0)
		return (angle + 2.0 * M_PI);
	else if (angle > 2.0 * M_PI)
		return (angle - 2.0 * M_PI);
	return (angle);
}

void	xpm_to_image(t_cub3d *env)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		env->texture[i].img = mlx_xpm_file_to_image(env->mlx,
				env->map->tex_path[i], &env->texture[i].width,
				&env->texture[i].height);
		if (!env->texture[i].img)
		{
			ft_putendl_fd("Error\nXpm file issue", 2);
			exit(1);
		}
		env->texture[i].data_addr = mlx_get_data_addr(env->texture[i].img,
				&env->texture[i].bpp, &env->texture[i].size_line,
				&env->texture[i].endian);
		i++;
	}
}
