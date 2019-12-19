/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 14:08:46 by sdiego            #+#    #+#             */
/*   Updated: 2019/12/04 14:08:47 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# include <unistd.h>
# include <stdlib.h>
# include <mlx.h>
# include <math.h>

# define WIN_W	960
# define WIN_H	600

//# define WIN_W	1920
//# define WIN_H	1080

# define VIEW_DIST	10

# define MAP_W 24
# define MAP_H 24

# define TEX_W 128
# define TEX_H 128

typedef struct		s_cast
{
	int		mx;
	
}					t_cast;

typedef struct	s_vec
{
	float		x;
	float		y;
}				t_vec;



typedef struct	s_ray
{
	float		x;


}				t_ray;

typedef struct	s_sprites
{
	double		x[10];
	double		y[10];
	int			sl[10];
	int			endl[10];
	char		*addimg[10];
	void		*image[10];
	int			bppimage[10];
	int			t_s[10];
}				t_sprites;

typedef struct	s_img
{
	void		*img;
	char		*ptr;
	int			bpp;
	int			stride;
	int			endian;
	int			sl[10];
	int			endl[10];
	char		*addimg[10];
	void		*image[10];
	int			bppimage[10];
	int			t_s[10];
	int			t_s_w;
	int			t_s_h;

}				t_img;

typedef	struct	s_mlx
{
	void		*mlx;
	void		*window;
	t_img		*i;
	t_sprites	*sprite;
	void		*img;
	char		**map;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	wall_dist[WIN_W];
	int		sing_tex;
	int		up;
}				t_mlx;

void			draw(t_mlx *m);

#endif