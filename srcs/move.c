/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:28:16 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/27 13:36:44 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	move_left(t_cub3d *env)
{
	t_ray	r;
	double	angle;
	double	straight;
	t_vd2d	len;
	t_vd2d	comp;

	angle = bound_angle(env->player.angle - (M_PI / 2.0));
	len.x = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	len.y = get_ray_len(env, (sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	straight = get_ray_len(env, angle, &r);
	comp.x = ft_abs(cos(angle)) * straight;
	comp.y = ft_abs(sin(angle)) * straight;
	if (len.x > MOVE_SPEED * 5.0 && (!r.vertical || comp.x > MOVE_SPEED * 5.0))
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (len.y > MOVE_SPEED * 5.0 && (r.vertical || comp.y > MOVE_SPEED * 5.0))
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_right(t_cub3d *env)
{
	t_ray	r;
	double	angle;
	double	straight;
	t_vd2d	len;
	t_vd2d	comp;

	angle = bound_angle(env->player.angle + (M_PI / 2.0));
	len.x = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	len.y = get_ray_len(env, (sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	straight = get_ray_len(env, angle, &r);
	comp.x = ft_abs(cos(angle)) * straight;
	comp.y = ft_abs(sin(angle)) * straight;
	if (len.x > MOVE_SPEED * 5.0 && (!r.vertical || comp.x > MOVE_SPEED * 5.0))
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (len.y > MOVE_SPEED * 5.0 && (r.vertical || comp.y > MOVE_SPEED * 5.0))
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_forward(t_cub3d *env)
{
	t_ray	r;
	double	angle;
	double	straight;
	t_vd2d	len;
	t_vd2d	comp;

	angle = env->player.angle;
	len.x = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	len.y = get_ray_len(env,
			(sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	straight = get_ray_len(env, angle, &r);
	comp.x = ft_abs(cos(angle)) * straight;
	comp.y = ft_abs(sin(angle)) * straight;
	if (len.x > MOVE_SPEED * 5.0 && (!r.vertical || comp.x > MOVE_SPEED * 5.0))
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (len.y > MOVE_SPEED * 5.0 && (r.vertical || comp.y > MOVE_SPEED * 5.0))
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}

void	move_backward(t_cub3d *env)
{
	t_ray	r;
	double	angle;
	double	straight;
	t_vd2d	len;
	t_vd2d	comp;

	angle = bound_angle(env->player.angle + M_PI);
	len.x = get_ray_len(env, (cos(angle) < 0) * M_PI, &r);
	len.y = get_ray_len(env, (sin(angle) < 0) * M_PI + (M_PI / 2.0), &r);
	straight = get_ray_len(env, angle, &r);
	comp.x = ft_abs(cos(angle)) * straight;
	comp.y = ft_abs(sin(angle)) * straight;
	if (len.x > MOVE_SPEED * 5.0 && (!r.vertical || comp.x > MOVE_SPEED * 5.0))
		env->player.pos.x += cos(angle) * MOVE_SPEED;
	if (len.y > MOVE_SPEED * 5.0 && (r.vertical || comp.y > MOVE_SPEED * 5.0))
		env->player.pos.y += sin(angle) * MOVE_SPEED;
}
