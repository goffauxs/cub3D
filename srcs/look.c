/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   look.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:44:12 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/27 13:38:30 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	mouse_down(int button, int x, int y, void *param)
{
	t_cub3d	*env;

	env = (t_cub3d *)param;
	(void)button;
	(void)x;
	(void)y;
	env->mouse_down = 1;
	return (0);
}

int	mouse_up(int button, int x, int y, void *param)
{
	t_cub3d	*env;

	env = (t_cub3d *)param;
	(void)button;
	(void)x;
	(void)y;
	env->mouse_down = 0;
	env->prev_x = 0;
	return (0);
}

int	mouse_move(int x, int y, void *param)
{
	t_cub3d	*env;
	int		dist;

	env = (t_cub3d *)param;
	(void)y;
	if (env->mouse_down)
	{
		if (env->prev_x)
		{
			dist = x - env->prev_x;
			env->player.angle -= dist * ((double)FOV / WIDTH) * (M_PI / 180.0);
			env->player.angle = bound_angle(env->player.angle);
		}
		env->prev_x = x;
	}
	return (0);
}

void	look_left(t_cub3d *env)
{
	env->player.angle = bound_angle(env->player.angle - LOOK_SPEED);
}

void	look_right(t_cub3d *env)
{
	env->player.angle = bound_angle(env->player.angle + LOOK_SPEED);
}
