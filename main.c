/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/29 15:55:25 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	player_angle(t_map *map, t_cub3d *env, int x, int y)
{
	if (map->array[y][x] == 'S')
		env->player.angle = M_PI * 0.5;
	else if (map->array[y][x] == 'N')
		env->player.angle = M_PI * 1.5;
	else if (map->array[y][x] == 'E')
		env->player.angle = 0.0;
	else if (map->array[y][x] == 'W')
		env->player.angle = M_PI;
}

static void	ft_player_pos(t_map *map, t_cub3d *env)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->array[y][x] == 'N' || map->array[y][x] == 'S'
				|| map->array[y][x] == 'E' || map->array[y][x] == 'W')
			{
				env->player.pos.x = x + 0.5;
				env->player.pos.y = y + 0.5;
				player_angle(map, env, x, y);
				break ;
			}
			x++;
		}
		y++;
	}
}

static int	inits_main(t_map *map, t_cub3d *env, t_text *text)
{
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIDTH, HEIGHT, "cub3D");
	text = malloc(sizeof(t_text) * 4);
	env->texture = text;
	env->map = map;
	xpm_to_image(env);
	env->img = mlx_new_image(env->mlx, WIDTH, HEIGHT);
	if (!env->img)
		return (-1);
	env->data_addr = mlx_get_data_addr(env->img, &env->bpp,
			&env->size_line, &env->endian);
	if (!env->data_addr)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_map	map;
	t_cub3d	env;
	t_text	*text;

	text = NULL;
	parsing_init(&map);
	if (cub_check(argc, argv, &map))
	{
		parsing_free(&map);
		exit(-1);
	}
	if (inits_main(&map, &env, text))
		exit(-1);
	mlx_hook(env.win, 17, 1L << 17, close_win, NULL);
	mlx_hook(env.win, 2, 1L << 0, key_down, &env);
	mlx_hook(env.win, 3, 1L << 1, key_up, &env);
	mlx_hook(env.win, 4, 1L << 2, mouse_down, &env);
	mlx_hook(env.win, 5, 1L << 3, mouse_up, &env);
	mlx_hook(env.win, 6, 0, mouse_move, &env);
	ft_player_pos(&map, &env);
	ft_bzero(&env.keys, sizeof(t_keys));
	env.prev_x = 0;
	mlx_loop_hook(env.mlx, on_update, &env);
	mlx_loop(env.mlx);
	free(env.texture);
}
