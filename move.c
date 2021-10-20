/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:28:16 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/20 15:47:27 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define MOVE_SPEED 0.1

void	move_left(t_cub3d *env)
{
	double	angle;
	int		x;
	int		y;
	int		x_delta;
	int		y_delta;
	
	angle = env->player.angle - (M_PI / 2.0);
	if (angle < 0.0)
		angle += 2.0 * M_PI;
	if (angle > 2.0 * M_PI)
		angle -= 2.0 * M_PI;
	x = (int)env->player.pos.x;
	y = (int)env->player.pos.y;
	x_delta = (int)(x + cos(angle));
	y_delta = (int)(y + sin(angle));
	if (env->map->array[y][x_delta] == '0')
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (env->map->array[y_delta][x] == '0')
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_right(t_cub3d *env)
{
	double	angle;
	int		x;
	int		y;
	int		x_delta;
	int		y_delta;

	angle = env->player.angle + (M_PI / 2.0);
	if (angle < 0.0)
		angle += 2.0 * M_PI;
	if (angle > 2.0 * M_PI)
		angle -= 2.0 * M_PI;
	x = (int)env->player.pos.x;
	y = (int)env->player.pos.y;
	x_delta = (int)(x + cos(angle));
	y_delta = (int)(y + sin(angle));
	if (env->map->array[y][x_delta] == '0')
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (env->map->array[y_delta][x] == '0')
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_forward(t_cub3d *env)
{
	int	x;
	int	y;
	int	x_delta;
	int	y_delta;

	x = (int)env->player.pos.x;
	y = (int)env->player.pos.y;
	x_delta = (int)(x + env->player.delta.x);
	y_delta = (int)(y + env->player.delta.y);
	if (env->map->array[y_delta][x] == '0')
		env->player.pos.y += env->player.delta.y * MOVE_SPEED;
	if (env->map->array[y][x_delta] == '0')
		env->player.pos.x += env->player.delta.x * MOVE_SPEED;
}

void	move_backward(t_cub3d *env)
{
	int	x;
	int	y;
	int	x_delta;
	int	y_delta;

	x = (int)env->player.pos.x;
	y = (int)env->player.pos.y;
	x_delta = (int)(x - env->player.delta.x);
	y_delta = (int)(y - env->player.delta.y);
	if (env->map->array[y_delta][x] == '0')
		env->player.pos.y -= env->player.delta.y * MOVE_SPEED;
	if (env->map->array[y][x_delta] == '0')
		env->player.pos.x -= env->player.delta.x * MOVE_SPEED;
}
