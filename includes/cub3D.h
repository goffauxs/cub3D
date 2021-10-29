/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 14:23:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/29 15:57:43 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "get_next_line.h"

# ifndef WIDTH
#  define WIDTH 1280
# endif
# ifndef HEIGHT
#  define HEIGHT 720
# endif
# define KEY_ESC 53
# define ARROW_LEFT 123
# define ARROW_RIGHT 124
# define W 13
# define A 0
# define S 1
# define D 2
# define TRUE 1
# define FALSE 0
# define EAST 0
# define SOUTH 1
# define WEST 2
# define NORTH 3

# define TILE_SIZE 32
# define FOV 60
# define DEG135 2.356194
# define DEG225 3.926991
# define MOVE_SPEED 0.1
# define LOOK_SPEED 0.06

typedef int		t_bool;

typedef struct s_map
{
	int		width;
	int		height;
	char	**array;
	char	*tex_path[4];
	int		floor[3];
	int		ceil[3];
}				t_map;

typedef struct s_vd2d
{
	double	x;
	double	y;
}				t_vd2d;

typedef struct s_vi2d
{
	int	x;
	int	y;
}				t_vi2d;

typedef struct s_ray
{
	t_vd2d	dir;
	t_vd2d	step_size;
	t_vd2d	start;
	t_vi2d	map_check;
	t_vd2d	len;
	t_vi2d	step;
	t_bool	tile_found;
	double	dist;
	t_vd2d	intersection;
	t_bool	vertical;
	double	line_len;
}				t_ray;

typedef struct s_player
{
	t_vd2d	pos;
	float	angle;
}				t_player;

typedef struct s_keys
{
	int	left;
	int	right;
	int	look_left;
	int	look_right;
	int	forward;
	int	backward;
}				t_keys;

typedef struct s_draw
{
	double	angle;
	double	ca;
	double	len;
}				t_draw;

typedef struct s_text
{
	void		*img;
	char		*data_addr;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
	int			endian;
}				t_text;

typedef struct s_cub3d
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
	t_text		*texture;
	int			prev_x;
	int			mouse_down;
}				t_cub3d;

/*
** parsing
*/
int		cub_check(int argc, char **argv, t_map *parsing);
void	parsing_free(t_map *parsing);
void	parsing_init(t_map *parsing);
int		check_map(t_map *parsing, char **file);
int		get_height(int *mapbeg, char **file);
int		check_char(char **file, int mapbeg);
int		get_max_len(char **file, int mapbeg);
int		parsing_allocation(t_map *parsing);
void	replace_space(int i, t_map *parsing, char *line);
int		check_args(int argc, char **argv);
int		check_textures(t_map *parsing, char **file);
int		check_floor(t_map *parsing, char **file);
char	**dup_cub(char **argv, char **ret);
void	free_split(char **split);
int		error(char *msg, char *free_ptr);
int		return_split_free(char **split);
int		check_open_wall(char **array, int y, int x);
char	**dup_map(char **array, int height);
void	replace_void(t_map *parsing);

void	ft_return_error(const char *err_msg, int system);
int		ft_check_valid(char *filename, t_map *map);
void	move_left(t_cub3d *env);
void	move_right(t_cub3d *env);
void	move_forward(t_cub3d *env);
void	move_backward(t_cub3d *env);
int		key_down(int keycode, void *param);
int		key_up(int keycode, void *param);
int		on_update(void *param);
void	ft_draw(t_cub3d *env);
int		close_win(void *param);
void	look_left(t_cub3d *env);
void	look_right(t_cub3d *env);
int		mouse_down(int button, int x, int y, void *param);
int		mouse_up(int button, int x, int y, void *param);
int		mouse_move(int x, int y, void *param);
double	get_ray_len(t_cub3d *env, double angle, t_ray *r);
double	ft_abs(double a);
double	bound_angle(double angle);

/*
** pixel.c
*/
void	ft_put_pixel(t_cub3d *env, int x, int y, int color);
int		ft_get_pixel(t_text *tex, int x, int y);

/*
** ray_casting.c
*/
double	get_ray_len(t_cub3d *env, double angle, t_ray *r);

/*
** draw.c
*/
void	xpm_to_image(t_cub3d *env);
void	ft_draw(t_cub3d *env);

void	draw_minimap(t_cub3d *env);

#endif