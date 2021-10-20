/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:35:58 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/20 11:12:16 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	close_win(void *param)
{
	(void)param;
	exit(0);
}

static void key_action(t_cub3d *env)
{
	if (env->keys.forward)
		move_forward(env);
	if (env->keys.backward)
		move_backward(env);
	if (env->keys.left)
		move_left(env);
	if (env->keys.right)
		move_right(env);
	if (env->keys.look_left)
		look_left(env);
	if (env->keys.look_right)
		look_right(env);
}

int	key_up(int keycode, void *param)
{
	t_cub3d *env;

	env = (t_cub3d *)param;
	if (keycode == ARROW_LEFT)
		env->keys.look_left = 0;
	else if (keycode == ARROW_RIGHT)
		env->keys.look_right = 0;
	else if (keycode == W)
		env->keys.forward = 0;
	else if (keycode == A)
		env->keys.left = 0;
	else if (keycode == S)
		env->keys.backward = 0;
	else if (keycode == D)
		env->keys.right = 0;
	return (0);
}

int	key_down(int keycode, void *param)
{
	t_cub3d	*env;

	env = (t_cub3d *)param;
	if (keycode == KEY_ESC)
		close_win(NULL);
	else if (keycode == ARROW_LEFT)
		env->keys.look_left = 1;
	else if (keycode == ARROW_RIGHT)
		env->keys.look_right = 1;
	else if (keycode == W)
		env->keys.forward = 1;
	else if (keycode == A)
		env->keys.left = 1;
	else if (keycode == S)
		env->keys.backward = 1;
	else if (keycode == D)
		env->keys.right = 1;
	return (0);
}

int	on_update(void *param)
{
	t_cub3d		*env;
	static int	frame_counter = 0;
	char		*counter_string;
	clock_t		end;
	double		time_spent;
	int			fps;

	env = (t_cub3d *)param;
	frame_counter++;
	end = clock();
	time_spent = (double)(end - g_begin) / CLOCKS_PER_SEC;
	if (time_spent > 1)
	{
		frame_counter = 0;
		g_begin = clock();
	}
	fps = (int)(frame_counter / time_spent);
	counter_string = ft_itoa(fps);
	ft_draw(env);
	key_action(env);
	//mlx_string_put(env->mlx, env->win, 10, 10, 0xFF0000, counter_string);
	free(counter_string);
	return (0);
}