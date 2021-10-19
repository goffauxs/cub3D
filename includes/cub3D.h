/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/19 13:58:44 by sgoffaux         ###   ########.fr       */
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
# include <time.h>

# ifndef WIDTH
#  define WIDTH 1024
# endif
# ifndef HEIGHT
#  define HEIGHT 720
# endif
# define MIN 0 - WIDTH / 2
# define MAX WIDTH / 2
# define KEY_ESC 53
# define ARROW_LEFT 123
# define ARROW_RIGHT 124
# define W 13
# define A 0
# define S 1
# define D 2

clock_t g_begin;

typedef struct	s_map
{
	int		width;
	int		height;
	char	**array;
}				t_map;

typedef struct	s_vd2d
{
	double	x;
	double	y;
}				t_vd2d;

typedef struct	s_vi2d
{
	int	x;
	int	y;
}				t_vi2d;

typedef struct	s_player
{
	t_vd2d	pos;
	t_vd2d	delta;
	float	angle;
}				t_player;

typedef struct	s_keys
{
	int	left;
	int	right;
	int	look_left;
	int	look_right;
	int	forward;
	int	backward;
}				t_keys;

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
	t_keys		keys;
}				t_cub3d;

void	ft_return_error(const char *err_msg, int system);
int		ft_check_valid(char *filename, t_map *map);
void	move_left(t_cub3d *env);
void	move_right(t_cub3d *env);
void	move_forward(t_cub3d *env);
void	move_backward(t_cub3d *env);
int		key_down(int keycode, void *param);
int		key_up(int keycode, void *param);
int		on_update(void *param);
void 	ft_draw(t_cub3d *env);
int		close_win(void *param);
void	look_left(t_cub3d *env);
void	look_right(t_cub3d *env);

#endif