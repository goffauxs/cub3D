/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:28:16 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/21 16:45:33 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define MOVE_SPEED 0.1

void	move_left(t_cub3d *env)
{
	t_ray	r;
	double	x_comp;
	double	y_comp;
	double	angle;

	angle = bound_angle(env->player.angle - (M_PI / 2.0));
	x_comp = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	y_comp = get_ray_len(env, (sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	if (x_comp > MOVE_SPEED * 5.0)
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (y_comp > MOVE_SPEED * 5.0)
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_right(t_cub3d *env)
{
	t_ray	r;
	double	x_comp;
	double	y_comp;
	double	angle;

	angle = bound_angle(env->player.angle + (M_PI / 2.0));
	x_comp = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	y_comp = get_ray_len(env, (sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	if (x_comp > MOVE_SPEED * 5.0)
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (y_comp > MOVE_SPEED * 5.0)
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_forward(t_cub3d *env)
{
	t_ray	r;
	double	x_comp;
	double	y_comp;

	x_comp = get_ray_len(env, (cos(env->player.angle) < 0) * M_PI, &r);
	y_comp = get_ray_len(env,
		(sin(env->player.angle) < 0) * M_PI + (M_PI / 2.0), &r);
	if (x_comp > MOVE_SPEED * 5.0)
		env->player.pos.x += env->player.delta.x * MOVE_SPEED;
	if (y_comp > MOVE_SPEED * 5.0)
		env->player.pos.y += env->player.delta.y * MOVE_SPEED;
}

void	move_backward(t_cub3d *env)
{
	t_ray	r;
	double	x_comp;
	double	y_comp;
	double	angle;

	angle = bound_angle(env->player.angle + M_PI);
	x_comp = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	y_comp = get_ray_len(env, (sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	if (x_comp > MOVE_SPEED * 5.0)
		env->player.pos.x -= env->player.delta.x * MOVE_SPEED;
	if (y_comp > MOVE_SPEED * 5.0)
		env->player.pos.y -= env->player.delta.y * MOVE_SPEED;
}
