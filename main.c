/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/18 16:37:34 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define TILE_SIZE 8
#define KEY_ESC 53
#define ARROW_LEFT 123
#define ARROW_RIGHT 124
#define W 13
#define A 0
#define S 1
#define D 2
#define DR 0.0005454153912

static void	ft_swap(double *a, double *b)
{
	double	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static double	ft_abs(double n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int	ft_ipart(double n)
{
	return ((int)n);
}

static int	close_win(void *param)
{
	(void)param;
	exit(0);
}

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

static void	ft_draw_line_loop(t_vd2d s, t_vd2d e, double gradient, t_cub3d *env, int color)
{
	double	inter_y;
	int		x;

	inter_y = (double)s.y;
	x = s.x;
	while (x <= e.x)
	{
		if (env->steep)
		{
			ft_put_pixel(env, ft_ipart(inter_y), x, color);
			ft_put_pixel(env, ft_ipart(inter_y) + 1, x, color);
		}
		else
		{
			ft_put_pixel(env, x, ft_ipart(inter_y), color);
			ft_put_pixel(env, x, ft_ipart(inter_y) + 1, color);
		}
		inter_y += gradient;
		x++;
	}
}

static void	ft_draw_line(t_vd2d s, t_vd2d e, t_cub3d *env, int color)
{
	double	dy;
	double	dx;
	double	gradient;

	env->steep = ft_abs(e.y - s.y) > ft_abs(e.x - s.x);
	if (env->steep)
	{
		ft_swap(&s.x, &s.y);
		ft_swap(&e.x, &e.y);
	}
	if (s.x > e.x)
	{
		ft_swap(&s.x, &e.x);
		ft_swap(&s.y, &e.y);
	}
	dx = (double)(e.x - s.x);
	dy = (double)(e.y - s.y);
	gradient = dy / dx;
	if (dx == 0.0)
		gradient = 1.0;
	ft_draw_line_loop(s, e, gradient, env, color);
}

static void ft_draw(t_cub3d *env)
{
	ft_bzero(env->data_addr, WIDTH * HEIGHT * (env->bpp / 8));
	for (int i = -960; i < 960; i++)
	{
		t_vd2d new_pos = {env->player.pos.x * TILE_SIZE, env->player.pos.y * TILE_SIZE};
		t_vd2d ray_dir = {(double)cos(env->player.angle + (i * DR)), (double)sin(env->player.angle + (i * DR))};
		t_vd2d ray_unit_step_size = {sqrt(1.0 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)), sqrt(1.0 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y))};
		t_vd2d ray_start = env->player.pos;
		t_vi2d map_check;
		map_check.x = (int)ray_start.x;
		map_check.y = (int)ray_start.y;
		t_vd2d ray_len;
		t_vi2d step;

		if (ray_dir.x < 0)
		{
			step.x = -1;
			ray_len.x = (ray_start.x - (double)map_check.x) * ray_unit_step_size.x;
		}
		else
		{
			step.x = 1;
			ray_len.x = ((double)(map_check.x + 1) - ray_start.x) * ray_unit_step_size.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			ray_len.y = (ray_start.y - (double)map_check.y) * ray_unit_step_size.y;
		}
		else
		{
			step.y = 1;
			ray_len.y = ((double)(map_check.y + 1) - ray_start.y) * ray_unit_step_size.y;
		}

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
			if (map_check.x >= 0 && map_check.x < env->map->width && map_check.y >= 0 && map_check.y < env->map->height)
			{
				if (env->map->array[map_check.y][map_check.x] == '1')
					b_tile_found = 1;
			}
		}

		t_vd2d intersection;
		if (b_tile_found)
		{
			intersection.x = ray_dir.x * dist + ray_start.x;
			intersection.y = ray_dir.y * dist + ray_start.y;
		}
		t_vd2d delta = {intersection.x * TILE_SIZE, intersection.y * TILE_SIZE};
		double len = sqrt((intersection.x - env->player.pos.x) * (intersection.x - env->player.pos.x) + (intersection.y - env->player.pos.y) * (intersection.y - env->player.pos.y));
		double ca = i * DR;
		if (ca < 0.0)
			ca += 2.0 * M_PI;
		if (ca > 2.0 * M_PI)
			ca -= 2.0 * M_PI;
		len = len * cos(ca);
		t_vd2d bottom = {(double)(i + 960), (double)(HEIGHT / 2.0 + ((double)HEIGHT / len) / 2)};
		t_vd2d top = {bottom.x, (double)(HEIGHT / 2.0 - ((double)HEIGHT / len) / 2)};
		if (vertical)
			ft_draw_line(bottom, top, env, 0xFFFF00);
		else
			ft_draw_line(bottom, top, env, 0xB5B500);
		ft_draw_line(new_pos, delta, env, 0xFFFF00);
	}
	for (int y = 0; y < env->map->height; y++)
	{
		for (int x = 0; x < env->map->width; x++)
		{
			for (int i = 0; i < TILE_SIZE; i++)
			{
				for (int j = 0; j < TILE_SIZE; j++)
				{
					if (env->map->array[y][x] == '1')
						ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0xFF0000);
					// if (env->map->array[y][x] == '0')
					// 	ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0x333333);
					if (env->map->array[y][x] == 'N' || env->map->array[y][x] == 'S' || env->map->array[y][x] == 'E' || env->map->array[y][x] == 'W')
						ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0x0000FF);
				}
			}
		}
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
}

int	key_hook(int keycode, void *param)
{
	t_cub3d	*env;

	env = (t_cub3d *)param;
	if (keycode == KEY_ESC)
		close_win(NULL);
	else if (keycode == ARROW_LEFT)
	{
		env->player.angle -= 0.05;
		if (env->player.angle < 0.0)
			env->player.angle += 2.0 * M_PI;
		env->player.delta.x = cos(env->player.angle);
		env->player.delta.y = sin(env->player.angle);
	}
	else if (keycode == ARROW_RIGHT)
	{
		env->player.angle += 0.05;
		if (env->player.angle > 2.0 * M_PI)
			env->player.angle -= 2.0 * M_PI;
		env->player.delta.x = cos(env->player.angle);
		env->player.delta.y = sin(env->player.angle);
	}
	else if (keycode == W)
	{
		if (env->map->array[(int)(env->player.pos.y + env->player.delta.y)][(int)env->player.pos.x] != '1')
			env->player.pos.y += env->player.delta.y * 0.25;
		if (env->map->array[(int)env->player.pos.y][(int)(env->player.pos.x + env->player.delta.x)] != '1')
			env->player.pos.x += env->player.delta.x * 0.25;
	}
	else if (keycode == S)
	{
		if (env->map->array[(int)(env->player.pos.y - env->player.delta.y)][(int)env->player.pos.x] != '1')
			env->player.pos.y -= env->player.delta.y * 0.25;
		if (env->map->array[(int)env->player.pos.y][(int)(env->player.pos.x - env->player.delta.x)] != '1')
			env->player.pos.x -= env->player.delta.x * 0.25;
	}
	else if (keycode == A)
	{
		double angle = env->player.angle + (M_PI / 2.0);
		if (angle < 0.0)
			angle += 2.0 * M_PI;
		if (angle > 2.0 * M_PI)
			angle -= 2.0 * M_PI;
		env->player.pos.x += cos(angle) * 0.25;
		angle = env->player.angle - (M_PI / 2.0);
		if (angle < 0.0)
			angle += 2.0 * M_PI;
		if (angle > 2.0 * M_PI)
			angle -= 2.0 * M_PI;
		env->player.pos.y += sin(angle) * 0.25;
		printf("left angle: %d\n", (int)(angle * (180.0 / M_PI)));
	}
	else if (keycode == D)
	{
		double angle = env->player.angle + (M_PI / 2.0);
		if (angle < 0.0)
			angle += 2.0 * M_PI;
		if (angle > 2.0 * M_PI)
			angle -= 2.0 * M_PI;
		env->player.pos.x -= cos(angle) * 0.25;
		angle = env->player.angle - (M_PI / 2.0);
		if (angle < 0.0)
			angle += 2.0 * M_PI;
		if (angle > 2.0 * M_PI)
			angle -= 2.0 * M_PI;
		env->player.pos.y -= sin(angle) * 0.25;
		printf("right angle: %d\n", (int)(angle * (180.0 / M_PI)));
	}
	printf("angle: %d\n", (int)(env->player.angle * (180.0 / M_PI)));
	ft_draw(env);
	return (0);
}

int main(int argc, char **argv)
{
	t_map	map;
	t_cub3d	env;

	if (argc != 2)
		exit(1);
	env.mlx = mlx_init();
	env.win = mlx_new_window(env.mlx, WIDTH, HEIGHT, "cub3D");
	mlx_hook(env.win, 17, 0, close_win, NULL);
	mlx_hook(env.win, 2, 0, key_hook, &env);
	ft_check_valid(argv[1], &map);
	env.img = mlx_new_image(env.mlx, WIDTH, HEIGHT);
	if (!env.img)
		return (1);
	env.data_addr = mlx_get_data_addr(env.img, &env.bpp, &env.size_line, &env.endian);
	if (!env.data_addr)
		return (1);
	env.map = &map;
	ft_bzero(env.data_addr, WIDTH * HEIGHT * (env.bpp / 8));
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
	ft_draw(&env);
	mlx_loop(env.mlx);
}