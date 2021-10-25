/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:04:11 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/25 15:12:32 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	ft_put_pixel(t_cub3d *env, int x, int y, int color)
{
	int	i;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		i = (x * env->bpp / 8) + (y * env->size_line);
		env->data_addr[i] = color;
		env->data_addr[++i] = color >> 8;
		env->data_addr[++i] = color >> 16;
	}
}

int	ft_get_pixel(t_text *tex, int x, int y)
{
	int		color;
	char	*dst;

	dst = tex->data_addr + (y * tex->size_line + x * (tex->bpp / 8));
	color = *(unsigned int *)dst;
	return (color);
}
