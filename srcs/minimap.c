/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 10:44:52 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/29 14:12:58 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

t_bool	sign(t_vd2d pt, t_vd2d p1, t_vd2d p2, t_vd2d p3)
{
	double	d1;
	double	d2;
	double	d3;
	t_bool	has_neg;
	t_bool	has_pos;

	d1 = (pt.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (pt.y - p2.y);
	d2 = (pt.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (pt.y - p3.y);
	d3 = (pt.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (pt.y - p1.y);
	has_neg = (d1 < 0 || d2 < 0 || d3 < 0);
	has_pos = (d1 > 0 || d2 > 0 || d3 > 0);
	return (!(has_neg && has_pos));
}

int	draw_triangle(t_player p, t_vd2d pt)
{
	t_vd2d	p1;
	t_vd2d	p2;
	t_vd2d	p3;

	p1.x = p.pos.x + cos(p.angle) * 0.5;
	p1.y = p.pos.y + sin(p.angle) * 0.5;
	p2.x = p.pos.x + cos(bound_angle(p.angle + DEG135)) * 0.5;
	p2.y = p.pos.y + sin(bound_angle(p.angle + DEG135)) * 0.5;
	p3.x = p.pos.x + cos(bound_angle(p.angle + DEG225)) * 0.5;
	p3.y = p.pos.y + sin(bound_angle(p.angle + DEG225)) * 0.5;
	return (sign(pt, p1, p2, p3));
}

void	pixel_minimap(t_cub3d *env, int x, int y, int tile)
{
	int		i;
	int		j;
	t_vd2d	pt;

	i = -1;
	while (++i < tile)
	{
		j = -1;
		while (++j < tile)
		{
			pt.x = x + (i * (1.0 / tile));
			pt.y = y + (j * (1.0 / tile));
			if (env->map->array[y][x] == '0' || env->map->array[y][x] == 'N'
				|| env->map->array[y][x] == 'S'
				|| env->map->array[y][x] == 'W'
				|| env->map->array[y][x] == 'E')
				ft_put_pixel(env, x * tile + i, y * tile + j, 0x696969);
			else if (env->map->array[y][x] == '1')
				ft_put_pixel(env, x * tile + i, y * tile + j, 0x333333);
			if (draw_triangle(env->player, pt))
				ft_put_pixel(env, x * tile + i, y * tile + j, 0xFF0000);
		}
	}
}

void	draw_minimap(t_cub3d *env)
{
	int		x;
	int		y;
	int		tile;

	if (env->map->height > env->map->width)
		tile = 300 / env->map->height;
	else
		tile = 300 / env->map->width;
	y = 0;
	while (y < env->map->height)
	{
		x = 0;
		while (x < env->map->width)
		{
			pixel_minimap(env, x, y, tile);
			x++;
		}
		y++;
	}
}
