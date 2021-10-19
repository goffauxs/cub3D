/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/19 14:18:12 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define TILE_SIZE 8
#define DR 0.0005454153912

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

void ft_draw(t_cub3d *env)
{
	// ft_bzero(env->data_addr, WIDTH * HEIGHT * (env->bpp / 8));
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
		t_vd2d ray_dir = {cos(env->player.angle + (i * DR)), sin(env->player.angle + (i * DR))};
		t_vd2d ray_unit_step_size = {sqrt(1.0 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)), sqrt(1.0 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y))};
		t_vd2d ray_start = env->player.pos;
		t_vi2d map_check;
		map_check.x = (int)ray_start.x;
		map_check.y = (int)ray_start.y;
		t_vd2d ray_len;
		t_vi2d step;

		if (ray_dir.x < 0)
			ray_len.x = (ray_start.x - (double)map_check.x) * ray_unit_step_size.x;
		else
			ray_len.x = ((double)(map_check.x + 1) - ray_start.x) * ray_unit_step_size.x;
		step.x = 1 - (ray_dir.x < 0) * 2;
		if (ray_dir.y < 0)
			ray_len.y = (ray_start.y - (double)map_check.y) * ray_unit_step_size.y;
		else
			ray_len.y = ((double)(map_check.y + 1) - ray_start.y) * ray_unit_step_size.y;
		step.y = 1 - (ray_dir.y < 0) * 2;

		int b_tile_found = 0;
		int	vertical = 0;
		double max_dist = 100.0;
		double dist = 0.0;
		while (!b_tile_found && dist < max_dist)
		{
			if (ray_len.x < ray_len.y)
			{
				map_check.x += step.x;
				dist = ray_len.x;
				vertical = 1;
				ray_len.x += ray_unit_step_size.x;
			}
			else
			{
				map_check.y += step.y;
				dist = ray_len.y;
				vertical = 0;
				ray_len.y += ray_unit_step_size.y;
			}
			if (map_check.x >= 0 && map_check.x < env->map->width
				&& map_check.y >= 0 && map_check.y < env->map->height)
				if (env->map->array[map_check.y][map_check.x] == '1')
					b_tile_found = 1;
		}

		t_vd2d intersection;
		if (b_tile_found)
		{
			intersection.x = ray_dir.x * dist + ray_start.x;
			intersection.y = ray_dir.y * dist + ray_start.y;
		}
		double len = sqrt((intersection.x - env->player.pos.x) * (intersection.x - env->player.pos.x) + (intersection.y - env->player.pos.y) * (intersection.y - env->player.pos.y));
		double ca = i * DR;
		if (ca < 0.0)
			ca += 2.0 * M_PI;
		if (ca > 2.0 * M_PI)
			ca -= 2.0 * M_PI;
		len = len * cos(ca);
		int	mid = HEIGHT / 2;
		int	line_len = (int)(HEIGHT / len * 2.5);
		int	bottom = mid - line_len / 2;
		int	top = mid + line_len / 2;
		bottom = (bottom > HEIGHT) ? HEIGHT : bottom;
		top = (top < 0) ? 0 : top;
		for (int j = bottom; j < top; j++)
		{
			if (vertical)
				ft_put_pixel(env, i + MAX, j, 0xFFFF00);
			else
				ft_put_pixel(env, i + MAX, j, 0xB5B500);
		}
	}
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