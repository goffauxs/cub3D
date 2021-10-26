/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:07:58 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/26 14:57:33 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
			ft_putendl_fd("Error: xpm file issue", 2);
			exit(0);
		}
		env->texture[i].data_addr = mlx_get_data_addr(env->texture[i].img,
				&env->texture[i].bpp, &env->texture[i].size_line,
				&env->texture[i].endian);
		i++;
	}
}

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
	step = env->texture[tex_idx].height / (double)r->line_len;
	while (j < (int)(HEIGHT / 2.0 + (r->line_len / 2.0)))
	{
		if (j < HEIGHT && j > 0)
			ft_put_pixel(env, k, j, ft_get_pixel(&env->texture[tex_idx],
					col, (int)i));
		j++;
		i += step;
	}
}

static void	draw_background(t_cub3d *env)
{
	int	x;
	int	y;

	y = 0;
	while (y++ < HEIGHT / 2)
	{
		x = 0;
		while (x++ < WIDTH)
			ft_put_pixel(env, x, y, (env->map->ceil[0] << 16
					| env->map->ceil[1] << 8 | env->map->ceil[2]));
	}
	y = HEIGHT / 2;
	while (y++ < HEIGHT)
	{
		x = 0;
		while (x++ < WIDTH)
			ft_put_pixel(env, x, y, (env->map->floor[0] << 16
					| env->map->floor[1] << 8 | env->map->floor[2]));
	}
}

void	ft_draw(t_cub3d *env)
{
	int		x;
	int		line_len;
	t_draw	draw;
	t_ray	ray;

	draw_background(env);
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
