/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:07 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/13 15:15:09 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	close_win(void *param)
{
	(void)param;
	exit(0);
}

int	key_hook(int keycode, void *param)
{
	(void)param;
	printf("keycode: %d\n", keycode);
	if (keycode == 53)
		close_win(NULL);
	return (0);
}

int main(int argc, char **argv)
{
	void	*mlx;
	void	*win;
	t_map	map;

	if (argc != 2)
		exit(1);
	mlx = mlx_init();
	win = mlx_new_window(mlx, WIDTH, HEIGHT, "cub3D");
	mlx_hook(win, 17, 0, close_win, NULL);
	mlx_key_hook(win, key_hook, NULL);
	ft_check_valid(argv[1], &map);
	mlx_loop(mlx);
}