/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   look.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:44:12 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/26 15:00:30 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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
