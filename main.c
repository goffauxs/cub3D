/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/20 15:47:14 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define TILE_SIZE 32
#define FOV 60
#define DR ((double)FOV / WIDTH) * (M_PI / 180.0)
// #define DR 0.001022653858588

static void	ft_put_pixel(t_cub3d *env, int x, int y, int color)
{
	int		i;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		i = (x * env->bpp / 8) + (y * env->size_line);
		env->data_addr[i] = color;
		env->data_addr[++i] = color >> 8;
		env->data_addr[++i] = color >> 16;
	}
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	init_ray(t_cub3d *env, t_ray *r, double offset)
{
	r->dir.x = cos(env->player.angle + offset);
	r->dir.y = sin(env->player.angle + offset);
	r->step_size.x = sqrt(1.0 + pow(r->dir.y / r->dir.x, 2));
	r->step_size.y = sqrt(1.0 + pow(r->dir.x / r->dir.y, 2));
	r->start.x = env->player.pos.x;
	r->start.y = env->player.pos.y;
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

double	get_ray_len(t_cub3d *env, double offset, t_ray *r)
{
	double	len;
	double	angle_correction;

	init_ray(env, r, offset);
	ray_loop(env, r);
	if (r->tile_found)
	{
		r->intersection.x = r->dir.x * r->dist + r->start.x;
		r->intersection.y = r->dir.y * r->dist + r->start.y;
	}
	len = sqrt(pow(r->intersection.x - r->start.x, 2)
			+ pow(r->intersection.y - r->start.y, 2));
	angle_correction = offset;
	if (angle_correction < 0)
		angle_correction += 2.0 * M_PI;
	else if (angle_correction > 2.0 * M_PI)
		angle_correction -= 2.0 * M_PI;
	return (len * cos(angle_correction));
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
		double len = get_ray_len(env, (double)i * DR, &ray);
		int	mid = HEIGHT / 2;
		int	line_len = (int)(1 / len * HEIGHT);
		int	bottom = mid - line_len / 2;
		int	top = mid + line_len / 2;
		bottom = (bottom > HEIGHT) ? HEIGHT : bottom;
		top = (top < 0) ? 0 : top;
		for (int j = bottom; j < top; j++)
		{
			if (ray.vertical)
				ft_put_pixel(env, i + MAX, j, 0xFFFF00);
			else
				ft_put_pixel(env, i + MAX, j, 0xB5B500);
		}
	}
	// for (int y = 0; y < env->map->height; y++)
	// {
	// 	for (int x = 0; x < env->map->width; x++)
	// 	{
	// 		for (int i = 0; i < TILE_SIZE; i++)
	// 		{
	// 			for (int j = 0; j < TILE_SIZE; j++)
	// 			{
	// 				if (env->map->array[y][x] == '1')
	// 					ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0xFF0000);
	// 				else if (env->map->array[y][x] == '0')
	// 					ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0x525252);
	// 			}
	// 		}
	// 	}
	// }
	//t_vd2d delta = {env->player.pos.x + cos(env->player.angle), env->player.pos.y + sin(env->player.angle)};
	//printf("pos: (%02.2f, %02.2f) | delta: (%02.2f, %02.2f) -- %f\n", env->player.pos.x * (double)TILE_SIZE, env->player.pos.y * (double)TILE_SIZE, delta.x * (double)TILE_SIZE, delta.y * (double)TILE_SIZE, env->player.angle);
	// for (int i = -5; i < 5; i++)
	// {
	// 	for (int j = -5; j < 5; j++)
	// 	{
	// 		ft_put_pixel(env, (env->player.pos.x * (double)TILE_SIZE) + i, (env->player.pos.y * (double)TILE_SIZE) + j, 0xFFFF00);
	// 	}
	// }
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
}

int main(int argc, char **argv)
{
	t_map	map;
	t_cub3d	env;

	if (argc != 2)
		exit(1);
	env.mlx = mlx_init();
	env.win = mlx_new_window(env.mlx, WIDTH, HEIGHT, "cub3D");
	mlx_hook(env.win, 17, 1L << 17, close_win, NULL);
	mlx_hook(env.win, 2, 1L << 0, key_down, &env);
	mlx_hook(env.win, 3, 1L << 1, key_up, &env);
	ft_check_valid(argv[1], &map);
	env.img = mlx_new_image(env.mlx, WIDTH, HEIGHT);
	if (!env.img)
		return (1);
	env.data_addr = mlx_get_data_addr(env.img, &env.bpp, &env.size_line, &env.endian);
	if (!env.data_addr)
		return (1);
	env.map = &map;
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
}