/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 15:04:15 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/21 11:51:13 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	ft_return_error(const char *err_msg, int system)
{
	if (!system)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	else
		perror(err_msg);
	exit(-1);
}

double	ft_abs(double a)
{
	if (a < 0.0)
		return (-a);
	return (a);
}

double	bound_angle(double angle)
{
	if (angle < 0)
		return (angle + 2.0 * M_PI);
	else if (angle > 2.0 * M_PI)
		return (angle - 2.0 * M_PI);
	return (angle);
}

