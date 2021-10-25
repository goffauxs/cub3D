/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:05:30 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/25 15:12:36 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

static void	init_ray(t_cub3d *env, t_ray *r, double angle)
{
	r->dir.x = cos(angle);
	r->dir.y = sin(angle);
	r->step_size.x = sqrt(1.0 + pow(r->dir.y / r->dir.x, 2));
	r->step_size.y = sqrt(1.0 + pow(r->dir.x / r->dir.y, 2));
	r->start = env->player.pos;
	r->map_check.x = (int)r->start.x;
	r->map_check.y = (int)r->start.y;
	r->tile_found = FALSE;
	r->vertical = FALSE;
	if (r->dir.x < 0)
		r->len.x = (r->start.x - (double)r->map_check.x) * r->step_size.x;
	else
		r->len.x = ((double)(r->map_check.x + 1) - r->start.x) * r->step_size.x;
	r->step.x = 1 - (r->dir.x < 0) * 2;
	if (r->dir.y < 0)
		r->len.y = (r->start.y - (double)r->map_check.y) * r->step_size.y;
	else
		r->len.y = ((double)(r->map_check.y + 1) - r->start.y) * r->step_size.y;
	r->step.y = 1 - (r->dir.y < 0) * 2;
}

static void	ray_loop(t_cub3d *env, t_ray *r)
{
	double	max_dist;

	max_dist = (double)ft_max(env->map->height, env->map->width);
	while (!r->tile_found && r->dist < max_dist)
	{
		if (r->len.x < r->len.y)
		{
			r->map_check.x += r->step.x;
			r->dist = r->len.x;
			r->vertical = TRUE;
			r->len.x += r->step_size.x;
		}
		else
		{
			r->map_check.y += r->step.y;
			r->dist = r->len.y;
			r->vertical = FALSE;
			r->len.y += r->step_size.y;
		}
		if (r->map_check.x >= 0 && r->map_check.x < env->map->width
			&& r->map_check.y >= 0 && r->map_check.y < env->map->height
			&& env->map->array[r->map_check.y][r->map_check.x] == '1')
			r->tile_found = TRUE;
	}
}

double	get_ray_len(t_cub3d *env, double angle, t_ray *r)
{
	double	len;

	init_ray(env, r, angle);
	ray_loop(env, r);
	if (r->tile_found)
	{
		r->intersection.x = r->dir.x * r->dist + r->start.x;
		r->intersection.y = r->dir.y * r->dist + r->start.y;
	}
	len = sqrt(pow(r->intersection.x - r->start.x, 2)
			+ pow(r->intersection.y - r->start.y, 2));
	return (len);
}
