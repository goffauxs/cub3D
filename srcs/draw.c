/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:07:58 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/26 15:42:02 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	get_dir(t_cub3d *env, t_ray *r, int *col, double angle)
{
	int	tex_idx;

	tex_idx = 0;
	if (r->vertical && cos(angle) < 0)
		tex_idx = WEST;
	else if (r->vertical)
		tex_idx = EAST;
	else if (!r->vertical && sin(angle) < 0)
		tex_idx = NORTH;
	else
		tex_idx = SOUTH;
	if (r->vertical && cos(angle) < 0)
		*col = (int)((1.0 - (r->intersection.y - (int)r->intersection.y))
				* (double)env->texture[tex_idx].width);
	else if (r->vertical)
		*col = (int)((r->intersection.y - (int)r->intersection.y)
				* (double)env->texture[tex_idx].width);
	else if (!r->vertical && sin(angle) < 0)
		*col = (int)((r->intersection.x - (int)r->intersection.x)
				* (double)env->texture[tex_idx].width);
	else
		*col = (int)((1.0 - (r->intersection.x - (int)r->intersection.x))
				* (double)env->texture[tex_idx].width);
	return (tex_idx);
}

static void	draw_floor(t_cub3d *env, int j, int k)
{
	while (j < HEIGHT)
	{
		ft_put_pixel(env, k, j, (env->map->floor[0] << 16
				| env->map->floor[1] << 8 | env->map->floor[2]));
		j++;
	}
}

static void	draw_ceil(t_cub3d *env, int j, int k)
{
	int	i;

	i = 0;
	while (i < j && j >= 0)
	{
		ft_put_pixel(env, k, i, (env->map->ceil[0] << 16
				| env->map->ceil[1] << 8 | env->map->ceil[2]));
		i++;
	}
}

static void	draw_mapped_texture(t_cub3d *env, double angle, t_ray *r, int k)
{
	double	i;
	int		j;
	int		tex_idx;
	double	step;
	int		col;

	tex_idx = get_dir(env, r, &col, angle);
	i = 0.0;
	j = (int)(HEIGHT / 2.0 - (r->line_len / 2.0));
	draw_ceil(env, j, k);
	step = env->texture[tex_idx].height / (double)r->line_len;
	while (j < (int)(HEIGHT / 2.0 + (r->line_len / 2.0)))
	{
		if (j <= HEIGHT && j >= 0)
			ft_put_pixel(env, k, j, ft_get_pixel(&env->texture[tex_idx],
					col, (int)i));
		j++;
		i += step;
	}
	draw_floor(env, j, k);
}

void	ft_draw(t_cub3d *env)
{
	int		x;
	int		line_len;
	t_draw	draw;
	t_ray	ray;

	x = 0 - WIDTH / 2;
	while (x++ < WIDTH / 2)
	{
		draw.angle = bound_angle(env->player.angle + (double)x
				* ((double)FOV / WIDTH) * (M_PI / 180.0));
		draw.ca = bound_angle((double)x
				* ((double)FOV / WIDTH) * (M_PI / 180.0));
		draw.len = get_ray_len(env, draw.angle, &ray);
		draw.len *= cos(draw.ca);
		line_len = (int)(atan2(0.5, draw.len) * (180.0 / M_PI)
				/ ((double)FOV / WIDTH) * 2.0);
		ray.line_len = line_len;
		draw_mapped_texture(env, draw.angle, &ray, x + WIDTH / 2);
	}
	draw_minimap(env);
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
}
