/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   look.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:44:12 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/25 14:12:37 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define LOOK_SPEED 0.04

void	look_left(t_cub3d *env)
{
	env->player.angle -= LOOK_SPEED;
	if (env->player.angle < 0.0)
		env->player.angle += 2.0 * M_PI;
	env->player.delta.x = cos(env->player.angle);
	env->player.delta.y = sin(env->player.angle);
}

void	look_right(t_cub3d *env)
{
	env->player.angle += LOOK_SPEED;
	if (env->player.angle > 2.0 * M_PI)
		env->player.angle -= 2.0 * M_PI;
	env->player.delta.x = cos(env->player.angle);
	env->player.delta.y = sin(env->player.angle);
}
