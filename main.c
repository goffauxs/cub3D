/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/14 15:01:37 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define TILE_SIZE 192
#define KEY_ESC 53
#define ARROW_LEFT 123
#define ARROW_RIGHT 124
#define W 13
#define A 0
#define S 1
#define D 2

static void	ft_swap(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static float	ft_abs(float n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int	ft_ipart(float n)
{
	return ((int)n);
}

static float	ft_fpart(float n)
{
	if (n > 0.f)
		return (n - ft_ipart(n));
	return (n - (ft_ipart(n) + 1.f));
}

static float	ft_rfpart(float n)
{
	return (1.f - ft_fpart(n));
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

static int	ft_get_color(float factor)
{
	int		r = 0xFF;
	int		g = 0xFF;
	int		b = 0x00;
	int		alpha;

	alpha = factor * 256;
	// r *= factor;
	// g *= factor;
	// b *= factor;
	return ((alpha << 24) | (r << 16) | (g << 8) | b);
}

static void	ft_draw_line_loop(t_vf2d s, t_vf2d e, float gradient, t_cub3d *env)
{
	float	inter_y;
	int		x;

	inter_y = (float)s.y;
	x = s.x;
	while (x <= e.x)
	{
		if (env->steep)
		{
			ft_put_pixel(env, ft_ipart(inter_y), x,
				ft_get_color(ft_rfpart(inter_y)));
			ft_put_pixel(env, ft_ipart(inter_y) + 1, x,
				ft_get_color(ft_fpart(inter_y)));
		}
		else
		{
			ft_put_pixel(env, x, ft_ipart(inter_y),
				ft_get_color(ft_rfpart(inter_y)));
			ft_put_pixel(env, x, ft_ipart(inter_y) + 1,
				ft_get_color(ft_fpart(inter_y)));
		}
		inter_y += gradient;
		x++;
	}
}

static void	ft_draw_line(t_vf2d s, t_vf2d e, t_cub3d *env)
{
	float	dy;
	float	dx;
	float	gradient;

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
	dx = (float)(e.x - s.x);
	dy = (float)(e.y - s.y);
	gradient = dy / dx;
	if (dx == 0.0f)
		gradient = 1.f;
	ft_draw_line_loop(s, e, gradient, env);
}

static void ft_draw(t_cub3d *env)
{
	ft_bzero(env->data_addr, WIDTH * HEIGHT * (env->bpp / 8));
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
					if (env->map->array[y][x] == '0')
						ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0x333333);
					if (env->map->array[y][x] == 'N' || env->map->array[y][x] == 'S' || env->map->array[y][x] == 'E' || env->map->array[y][x] == 'W')
						ft_put_pixel(env, (x * TILE_SIZE) + i, (y * TILE_SIZE) + j, 0x0000FF);
				}
			}
		}
	}
	t_vf2d new_pos = {env->player.pos.x * TILE_SIZE, env->player.pos.y * TILE_SIZE};
	t_vf2d delta = {(env->player.pos.x + env->player.delta.x * 10) * TILE_SIZE, (env->player.pos.y + env->player.delta.y * 10) * TILE_SIZE};
	t_vf2d ray_dir = env->player.delta;
	t_vf2d ray_unit_step_size = {sqrt(1 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)), sqrt(1 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y))};
	t_vf2d ray_start = env->player.pos;
	t_vi2d map_check;
	map_check.x = (int)ray_start.x;
	map_check.y = (int)ray_start.y;
	t_vf2d ray_len;
	t_vi2d step;

	printf("ray_dir: (%02.2f, %02.2f)\n", ray_dir.x, ray_dir.y);
	if (ray_dir.x < 0)
	{
		step.x = -1;
		ray_len.x = (ray_start.x - (float)map_check.x) * ray_unit_step_size.x;
	}
	else
	{
		step.x = 1;
		ray_len.x = ((float)(map_check.x + 1) - ray_start.x) * ray_unit_step_size.x;
	}
	if (ray_dir.y < 0)
	{
		step.y = -1;
		ray_len.y = (ray_start.y - (float)map_check.y) * ray_unit_step_size.y;
	}
	else
	{
		step.y = 1;
		ray_len.y = ((float)(map_check.y + 1) - ray_start.y) * ray_unit_step_size.x;
	}

	printf("ray_len: (%02.2f, %02.2f)\n", ray_len.x, ray_len.y);
	int b_tile_found = 0;
	float max_dist = 100.f;
	float dist = 0.f;
	while (!b_tile_found && dist < max_dist)
	{
		if (ray_len.x < ray_len.y)
		{
			map_check.x += step.x;
			dist = ray_len.x;
			ray_len.x += ray_unit_step_size.x;
		}
		else
		{
			map_check.y += step.y;
			dist = ray_len.y;
			ray_len.y += ray_unit_step_size.y;
		}
		if (map_check.x >= 0 && map_check.x < env->map->width && map_check.y >= 0 && map_check.y < env->map->height)
		{
			if (env->map->array[map_check.y][map_check.x] == '1')
				b_tile_found = 1;
		}
	}

	t_vf2d intersection;
	if (b_tile_found)
	{
		intersection.x = ray_dir.x * dist + ray_start.x;
		intersection.y = ray_dir.y * dist + ray_start.y;
	}
	printf("intersection: (%02.2f, %02.2f)\n", intersection.x, intersection.y);
	// printf("pos: (%02.2f, %02.2f)\n", new_pos.x, new_pos.y);
	// printf("delta: (%02.2f, %02.2f)\n", delta.x, delta.y);
	ft_draw_line(new_pos, delta, env);
	for (int y = -5; y < 5; y++)
	{
		for (int x = -5; x < 5; x++)
		{
			ft_put_pixel(env, (intersection.x * TILE_SIZE) + x, (intersection.y * TILE_SIZE) + y, 0x00FF00);
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
		env->player.angle -= 0.05f;
		if (env->player.angle > 2 * M_PI)
			env->player.angle -= 2 * M_PI;
		env->player.delta.x = cos(env->player.angle);
		env->player.delta.y = sin(env->player.angle);
	}
	else if (keycode == ARROW_RIGHT)
	{
		env->player.angle += 0.05f;
		if (env->player.angle < 0)
			env->player.angle += 2 * M_PI;
		env->player.delta.x = cos(env->player.angle);
		env->player.delta.y = sin(env->player.angle);
	}
	else if (keycode == W)
	{
		env->player.pos.x += env->player.delta.x * 0.25f;
		env->player.pos.y += env->player.delta.y * 0.25f;
	}
	else if (keycode == S)
	{
		env->player.pos.x -= env->player.delta.x * 0.25f;
		env->player.pos.y -= env->player.delta.y * 0.25f;
	}
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
	mlx_do_key_autorepeaton(env.mlx);
	mlx_hook(env.win, 17, 0, close_win, NULL);
	mlx_key_hook(env.win, key_hook, &env);
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
				env.player.pos.x = x + 0.5f;
				env.player.pos.y = y + 0.5f;
				if (map.array[y][x] == 'S')
					env.player.angle = M_PI * 0.5f;
				else if (map.array[y][x] == 'N')
					env.player.angle = M_PI * 1.5f;
				else if (map.array[y][x] == 'E')
					env.player.angle = 0.f;
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