/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/14 14:34:01 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"

# define WIDTH 1920
# define HEIGHT 1080

typedef struct	s_map
{
	int		width;
	int		height;
	char	**array;
}				t_map;

typedef struct	s_vf2d
{
	float	x;
	float	y;
}				t_vf2d;

typedef struct	s_vi2d
{
	int	x;
	int	y;
}				t_vi2d;

typedef struct	s_player
{
	t_vf2d	pos;
	t_vf2d	delta;
	float	angle;
}				t_player;

typedef struct	s_cub3d
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*data_addr;
	int			bpp;
	int			size_line;
	int			endian;
	int			steep;
	t_map		*map;
	t_player	player;
}				t_cub3d;

void	ft_return_error(const char *err_msg, int system);
int		ft_check_valid(char *filename, t_map *map);

#endif