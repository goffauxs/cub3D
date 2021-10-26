/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:07:58 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/26 12:04:23 by sgoffaux         ###   ########.fr       */
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

double sign (t_vd2d pt, t_vd2d p1, t_vd2d p2)
{
    return (pt.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (pt.y - p2.y);
}

int	draw_triangle(t_cub3d *env, t_vd2d pt)
{
	t_vd2d	p1;
	t_vd2d	p2;
	t_vd2d	p3;

	p1.x = env->player.pos.x + cos(env->player.angle) * 0.5;
	p1.y = env->player.pos.y + sin(env->player.angle) * 0.5;
	p2.x = env->player.pos.x + cos(env->player.angle + bound_angle(135.0 * (M_PI / 180.0))) * 0.5;
	p2.y = env->player.pos.y + sin(env->player.angle + bound_angle(135.0 * (M_PI / 180.0))) * 0.5;
	p3.x = env->player.pos.x + cos(env->player.angle + bound_angle(225.0 * (M_PI / 180.0))) * 0.5;
	p3.y = env->player.pos.y + sin(env->player.angle + bound_angle(225.0 * (M_PI / 180.0))) * 0.5;

	float d1, d2, d3;
    t_bool has_neg, has_pos;

    d1 = sign(pt, p1, p2);
    d2 = sign(pt, p2, p3);
    d3 = sign(pt, p3, p1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
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
	for (int y = 0; y < env->map->height; y++)
	{
		for (int x = 0; x < env->map->width; x++)
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					t_vd2d pt = {x + (i * 0.2), y + (j * 0.2)};
					if (env->map->array[y][x] == '0' || env->map->array[y][x] == 'N' || env->map->array[y][x] == 'S' || env->map->array[y][x] == 'W' || env->map->array[y][x] == 'E')
						ft_put_pixel(env, x * 5 + i, y * 5 + j, 0x696969);
					else if (env->map->array[y][x] == '1')
						ft_put_pixel(env, x * 5 + i, y * 5 + j, 0x333333);
					if (draw_triangle(env, pt))
						ft_put_pixel(env, x * 5 + i, y * 5 + j, 0xFF0000);
				}
			}
		}
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
}
