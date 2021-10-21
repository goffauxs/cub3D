/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/21 16:40:06 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define TILE_SIZE 32
#define FOV 60
#define DR ((double)FOV / WIDTH) * (M_PI / 180.0)

static void	ft_put_pixel(t_cub3d *env, int x, int y, int color)
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

static int	ft_get_pixel(t_text *tex, int x, int y)
{
	int		color;
	char	*dst;

	dst = tex->data_addr + (y * tex->size_line + x * (tex->bpp / 8));
	color = *(unsigned int *)dst;
	return (color);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	init_ray(t_cub3d *env, t_ray *r, double angle)
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

void	ray_loop(t_cub3d *env, t_ray *r)
{
	double max_dist;

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

void xpm_to_image(t_cub3d *env)
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

void	draw_mapped_texture(t_cub3d *env, double angle, int line_len, t_ray *r, int k)
{
	double	i;
	int		j;
	int		tex_idx;
	int		color;
	double 	step;
	int		col;

	if (r->vertical)
	{
		if (sin(angle) < 0)
			tex_idx = NORTH;
		else
			tex_idx = SOUTH;
		col = (int)((r->intersection.y - (int)r->intersection.y) * (double)env->texture[tex_idx].width);
	}
	else
	{
		if (cos(angle) < 0)
			tex_idx = WEST;
		else
			tex_idx = EAST;
		col = (int)((r->intersection.x - (int)r->intersection.x) * (double)env->texture[tex_idx].width);
	}
	i = 0.0;
	j = HEIGHT / 2.0 - (line_len / 2.0);
	step = env->texture[tex_idx].height / (double)line_len;
	// printf("tex width: %d | tex_height: %d\n", env->texture[tex_idx].width, env->texture[tex_idx].height);
	while (i < env->texture[tex_idx].height)
	{
		//printf("i: %f | step: %f | col: %d\n", i, step, col);
		color = ft_get_pixel(&env->texture[tex_idx], col, (int)i);
		ft_put_pixel(env, k, j, color);
		j++;
		i += step;
	}
}

void ft_draw(t_cub3d *env)
{
	for (int y = 0; y < HEIGHT / 2; y++)
	{
		for (int x = 0; x < WIDTH; x++)
			ft_put_pixel(env, x, y, 0x45b5ed);
	}
	for (int y = HEIGHT / 2; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
			ft_put_pixel(env, x, y, 0xa37000);
	}
	for (int i = MIN; i < MAX; i++)
	{
		t_ray	ray;
		double angle = bound_angle(env->player.angle + (double)i * DR);
		double ca = bound_angle((double)i * DR);
		double len = get_ray_len(env, angle, &ray);
		len *= cos(ca);
		int	line_len = (int)(atan2(0.5, len) * (180.0 / M_PI) / ((double)FOV / WIDTH) * 2.0);
		// for (int j = mid - line_len; j < mid + line_len; j++)
		// {
		// 	if (ray.vertical)
		// 		ft_put_pixel(env, i + MAX, j, 0xFFFF00);
		// 	else
		// 		ft_put_pixel(env, i + MAX, j, 0xB5B500);
		// }
		draw_mapped_texture(env, angle, line_len, &ray, i + MAX);
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
}

int main(int argc, char **argv)
{
	t_map	map;
	t_cub3d	env;
	t_text	*text;

	if (argc != 2)
		exit(1);
	parsing_init(&map);
	if (cub_check(argc, argv, &map))
	{
		parsing_free(&map);
		exit(-1);
	}
	env.mlx = mlx_init();
	env.win = mlx_new_window(env.mlx, WIDTH, HEIGHT, "cub3D");
	text = malloc(sizeof(t_text) * 4);
	env.texture = text;
	env.map = &map;
	xpm_to_image(&env);
	mlx_hook(env.win, 17, 1L << 17, close_win, NULL);
	mlx_hook(env.win, 2, 1L << 0, key_down, &env);
	mlx_hook(env.win, 3, 1L << 1, key_up, &env);
	env.img = mlx_new_image(env.mlx, WIDTH, HEIGHT);
	if (!env.img)
		return (1);
	env.data_addr = mlx_get_data_addr(env.img, &env.bpp, &env.size_line, &env.endian);
	if (!env.data_addr)
		return (1);
	for (int y = 0; y < map.height; y++)
	{
		for (int x = 0; x < map.width; x++)
		{
			if (map.array[y][x] == 'N' || map.array[y][x] == 'S' || map.array[y][x] == 'E' || map.array[y][x] == 'W')
			{
				env.player.pos.x = x + 0.5;
				env.player.pos.y = y + 0.5;
				if (map.array[y][x] == 'S')
					env.player.angle = M_PI * 0.5;
				else if (map.array[y][x] == 'N')
					env.player.angle = M_PI * 1.5;
				else if (map.array[y][x] == 'E')
					env.player.angle = 0.0;
				else if (map.array[y][x] == 'W')
					env.player.angle = M_PI;
				env.player.delta.x = cos(env.player.angle);
				env.player.delta.y = sin(env.player.angle);
				break ;
			}
		}
	}
	g_begin = clock();
	mlx_loop_hook(env.mlx, on_update, &env);
	mlx_loop(env.mlx);
	free(env.texture);
}