/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 14:14:56 by sdiego            #+#    #+#             */
/*   Updated: 2019/12/04 14:14:57 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <stdio.h>

void	draw_tex(int x, int y, int color, t_mlx *m)
{
	int i;
	i = 0;
	while (i <= y)
	{
		x += WIN_W;
		i++;
	}
	if (x >= WIN_W * WIN_H)
		x = WIN_W * WIN_H;
	*(int *)(m->i->ptr + x * m->i->bpp) = color;
}

int		get_color_tex(int x, int y, char *addimg, int t_s)
{
	int color;
	int temp;

	temp = (x + y * t_s) * 4;
	color = *(int*)(addimg + temp);
	
	return (color);
}

void	wasd(int key, t_mlx *m)
{
	double	speed = 0.05;
	double	speed_move = 0.25;

	if (key == 13)
	{
		if (m->map[(int)(m->pos_x + m->dir_x * (speed_move * 2))][(int)m->pos_y] <= 0)
			m->pos_x += m->dir_x * speed_move;
		if (m->map[(int)m->pos_x][(int)(m->pos_y + m->dir_y * (speed_move * 2))] <= 0)
			m->pos_y += m->dir_y * speed_move;
	}
	else if (key == 1)
	{
		if (m->map[(int)(m->pos_x - m->dir_x * (speed_move * 2))][(int)m->pos_y] <= 0)
			m->pos_x -= m->dir_x * speed_move;
		if (m->map[(int)m->pos_x][(int)(m->pos_y - m->dir_y * (speed_move * 2))] <= 0)
			m->pos_y -= m->dir_y * speed_move;
	}
	else if (key == 2)
	{
		double	old_dir_x = m->dir_x;
		double	old_plane_x = m->plane_x;

		m->dir_x = m->dir_x * cos(-speed) - m->dir_y * sin(-speed);
		m->dir_y = old_dir_x * sin(-speed) + m->dir_y * cos(-speed);
		m->plane_x = m->plane_x * cos(-speed) - m->plane_y * sin(-speed);
		m->plane_y = old_plane_x * sin(-speed) + m->plane_y * cos(-speed);
		m->sing_tex += 20;
		if (m->sing_tex >= 700)
			m->sing_tex = 0;
	}
	else if (key == 0)
	{
		double	old_dir_x = m->dir_x;
		double	old_plane_x = m->plane_x;

		m->dir_x = m->dir_x * cos(speed) - m->dir_y * sin(speed);
		m->dir_y = old_dir_x * sin(speed) + m->dir_y * cos(speed);
		m->plane_x = m->plane_x * cos(speed) - m->plane_y * sin(speed);
		m->plane_y = old_plane_x * sin(speed) + m->plane_y * cos(speed);
		m->sing_tex -= 20;
		if (m->sing_tex <= 0)
			m->sing_tex = 700;
	}
}


int		close_win(void *param)
{
	t_mlx	*m;

	m = (t_mlx*)param;
	exit(0);
}

void	up_down(int key, t_mlx *m)
{
	if (key == 126)
	{
		m->up += 5;
	}
	else if (key == 125)
	{
		m->up -= 5;
	}
}

int		key_press(int key, void *param)
{
	t_mlx	*m;

	m = (t_mlx*)param;
	if (key == 53)
		close_win(param);
	else if (key == 0 || key == 1 || key == 2 || key == 13)
		wasd(key, m);
	else if (key == 126 || key == 125)
		up_down(key, m);
	draw(m);
	return (0);
}

void	draw_ver_line(int x, int a, int b, int color, t_img *img)
{
	int i;
	i = 0;
	//printf("a= %i\n", a);
	//printf("b= %i\n", b);
	while (i <= a)
	{
		x += WIN_W;
		i++;
	}
	while (a <= b)
	{
		*(int *)(img->ptr + x * img->bpp) = color;
		x += WIN_W;
		a++;
	}
}

void	clear_map(t_mlx *m)
{
	int color;
	int	i;

	color = 0x000000;
	i = 0;
	while (i <= WIN_H * WIN_W)
	{
		*(int *)(m->i->ptr + i * m->i->bpp) = color;
		i++;
	}
}

void	draw(t_mlx *m)
{
	/*int		world_map[MAP_W][MAP_H] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}	
	};*/

	clear_map(m);
	int x = 0;
	while (x < WIN_W)
	{
		double	camera_x = 2 * x / (double)WIN_W - 1; // координата x на плоскости камеры
		double	ray_dir_x = m->dir_x + m->plane_x * camera_x; // направление луча по x
		double	ray_dir_y = m->dir_y + m->plane_y * camera_x; // направление луча по y

		

		int	map_x = (int)m->pos_x; // в каком квадрате карты мы находимся
		int	map_y = (int)m->pos_y; // в каком квадрате карты мы находимся

		double side_dist_x; // изначальная длина луча от тек. позиции до следующей
		double side_dist_y; // изначальная длина луча от тек. позиции до следующей

		double delta_dist_x = sqrt(1 + (ray_dir_y * ray_dir_y) / (ray_dir_x * ray_dir_x)); // длина луча от одной стороны x до следующей
		double delta_dist_y = sqrt(1 + (ray_dir_x * ray_dir_x) / (ray_dir_y * ray_dir_y)); // длина луча от одной стороны y до следующей
		double perp_wall_dist; // расчет длины луча до стены 

		int step_x; // движемся по x-направлению (+1)(-1)
		int step_y; // движемся по y-направлению (+1)(-1)

		int hit = 0; // было ли попадание в стену?
		int side; // по какой стороне стены было попадание? по x=0 , по y=1

		// шагаем по x или по y
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (m->pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - m->pos_x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (m->pos_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - m->pos_y) * delta_dist_y;
		}

		while (hit == 0) // алгоритм DDA
		{
			if (side_dist_x < side_dist_y) // переходим к след. квадрату по x или по y
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (m->map[map_x][map_y] > 0) // проверяем попал ли луч в стену 
			{
				hit = 1;
			}
		}

		// вычисляем расстояние до стены 
		if (side == 0) // если стена x
		{
			perp_wall_dist = (map_x - m->pos_x + (1 - step_x) / 2) / ray_dir_x;
		}
		else
		{
			perp_wall_dist = (map_y - m->pos_y + (1 - step_y) / 2) / ray_dir_y;
		}
		
		int	h = WIN_H;
		int	wall_height = (int) (h / perp_wall_dist); //вычисляем нижний и верхний пиксель стены
		int	draw_start = (-wall_height / 2 + h / 2) + m->up;
		if (draw_start < 0)
			draw_start = 0;
		int	draw_end = (wall_height / 2 + h / 2) + m->up;
		if (draw_end >= h)
			draw_end = h - 1;


		//if (draw_end_tex >= h)
		//draw_end_tex = h - 1;
/*
		// выбираем цвет стен 
		int color;
		if (world_map[map_x][map_y] == 1)
		{
			color = 0xf0e68c; // бежевый 
		}
		else if (world_map[map_x][map_y] == 2)
		{
			color = 0xff00; // green
		}
		else if (world_map[map_x][map_y] == 3)
		{
			color = 0xffffff; // white
		}
		else if (world_map[map_x][map_y] == 4)
		{
			color = 0xff00ff; // magenta
		}
		else if (world_map[map_x][map_y] == 5)
		{
			color = 0xff7f24; // chocolate
		}

		if (side == 1) // если сторона y то уменьшаем яркость в 2 раза
		{
			color = color / 2; 
		}
*/

		double wall_x; //где именно было попадание в стену
		if (side == 0)
			wall_x = m->pos_y + perp_wall_dist * ray_dir_y;
		else
		{
			wall_x = m->pos_x + perp_wall_dist * ray_dir_x;
		}
		wall_x -= floor((wall_x)); // координата х на текстуре
		int tex_x = (int)(wall_x * (double)TEX_W);
		if (side == 0 && ray_dir_x > 0)
			tex_x = TEX_W - tex_x - 1;
		if (side == 1 && ray_dir_y < 0)
			tex_x = TEX_W - tex_x - 1;
		
		int	y = draw_start;
		int color;
		while (y < draw_end)
		{
			int d = (y - m->up) * 256 - h * 128 + wall_height * 128;
			int tex_y = ((d * TEX_H) / wall_height) / 256;
			if (tex_x <= 0)
				tex_x = 0;
			if (tex_y <= 0)
				tex_y = 0;
			if (tex_x >= TEX_W)
				tex_x = TEX_W;
			if (tex_y >= TEX_H)
				tex_y = TEX_H;
			
			if (side == 0 && ray_dir_x > 0)
				color = get_color_tex(tex_x, tex_y, m->i->addimg[2], m->i->t_s[2]);
			if (side == 0 && ray_dir_x < 0)
				color = get_color_tex(tex_x, tex_y, m->i->addimg[4], m->i->t_s[4]);
			if (side == 1 && ray_dir_y > 0)
				color = get_color_tex(tex_x, tex_y, m->i->addimg[5], m->i->t_s[5]);
			if (side == 1 && ray_dir_y < 0)
				color = get_color_tex(tex_x, tex_y, m->i->addimg[6], m->i->t_s[6]);
			draw_tex(x, y, color, m);
			y++;
		}
		



		

		// skybox
		int c = m->sing_tex + x;
		y = 0;
		while (draw_start > y)
		{
				
			color = get_color_tex(c, y + 350 - m->up, m->i->addimg[3], m->i->t_s_w);
			draw_tex(x, y, color, m);
			y++;
		}






		// пол и потолок
		double floor_x_wall; // положение пола в нижней части стены
		double floor_y_wall; // положение пола в нижней части стены
		
		// 4 различных направления стены
		if (side == 0 && ray_dir_x > 0)
		{
			floor_x_wall = map_x;
			floor_y_wall = map_y + wall_x;
		}
		else if (side == 0 && ray_dir_x < 0)
		{
			floor_x_wall = map_x + 1;
			floor_y_wall = map_y + wall_x;
		}
		else if (side == 1 && ray_dir_y > 0)
		{
			floor_x_wall = map_x + wall_x;
			floor_y_wall = map_y;
		}
		else if (side == 1 && ray_dir_y < 0)
		{
			floor_x_wall = map_x + wall_x;
			floor_y_wall = map_y + 1;
		}

		double	dist_player; 
		double	current_dis; // текущее расстояние 

		dist_player = 0.0;
		
		/*if (draw_end < 0) // избежания переполнения 
			draw_end = h;
		*/
		y = draw_end;
		while (y <= h)
		{
			current_dis = h / (2.0 * (y - m->up) - h);
			double	floor_weight = (current_dis - dist_player) / (perp_wall_dist - dist_player);
			double	current_floor_x = floor_weight * floor_x_wall + (1 - floor_weight) * m->pos_x; // координата квадрата на карте
			double	current_floor_y = floor_weight * floor_y_wall + (1 - floor_weight) * m->pos_y; // координата квадрата на карте
			//printf("y = %f\n", floor_weight);
			int	floor_x_tex;
			int	floor_y_tex;

			floor_x_tex = (int)(current_floor_x * TEX_W) % TEX_W;
			floor_y_tex = (int)(current_floor_y * TEX_H) % TEX_H;
			if (floor_x_tex <= 0)
				floor_x_tex = 0;
			if (floor_y_tex <= 0)
				floor_y_tex = 0;
			if (floor_x_tex >= 128)
				floor_x_tex = 128;
			if (floor_y_tex >= 128)
				floor_y_tex = 128;

			color = get_color_tex(floor_x_tex, floor_y_tex, m->i->addimg[1], m->i->t_s[1]);
			draw_tex(x, y, color, m);

			/*if (m->map[(int)current_floor_x][(int)current_floor_y] == -1)
			{
				color = get_color_tex(floor_x_tex, floor_y_tex, m->i->addimg[0], m->i->t_s[0]);
				draw_tex(x, h - y, color, m);
			}*/
			y++;
		}

		if (side == 0 && ray_dir_x > 0)
		{
			floor_x_wall = map_x;
			floor_y_wall = map_y + wall_x;
		}
		else if (side == 0 && ray_dir_x < 0)
		{
			floor_x_wall = map_x + 1;
			floor_y_wall = map_y + wall_x;
		}
		else if (side == 1 && ray_dir_y > 0)
		{
			floor_x_wall = map_x + wall_x;
			floor_y_wall = map_y;
		}
		else if (side == 1 && ray_dir_y < 0)
		{
			floor_x_wall = map_x + wall_x;
			floor_y_wall = map_y + 1;
		}
		
		y = 0;
		while (y <= draw_start)
		{
			current_dis = h / (2.0 * (y - m->up) - h);
			double	floor_weight = (current_dis - dist_player) / (perp_wall_dist - dist_player);
			double	current_floor_x = floor_weight * floor_x_wall + (1 - floor_weight) * m->pos_x; // координата квадрата на карте
			double	current_floor_y = floor_weight * floor_y_wall + (1 - floor_weight) * m->pos_y; // координата квадрата на карте

			//printf("x = %f\n", floor_weight);
			int	floor_x_tex;
			int	floor_y_tex;

			floor_x_tex = (int)(current_floor_x * TEX_W) % TEX_W;
			floor_y_tex = (int)(current_floor_y * TEX_H) % TEX_H;
			if (floor_x_tex <= 0)
				floor_x_tex = 0;
			if (floor_y_tex <= 0)
				floor_y_tex = 0;
			if (floor_x_tex >= 128)
				floor_x_tex = 128;
			if (floor_y_tex >= 128)
				floor_y_tex = 128;
			
		
			
				color = get_color_tex(floor_x_tex, floor_y_tex, m->i->addimg[0], m->i->t_s[0]);
				draw_tex(x, y, color, m);
			
			y++;
		}



		m->wall_dist[x] = perp_wall_dist; // для спрайтов
		x++;
	}
	
	// sprites

	int	h = WIN_H;
	double sprite_x = m->sprite->x[0] - m->pos_x; // положение спрайта относительно камеры
	double sprite_y = m->sprite->y[0] - m->pos_y; // положение спрайта относительно камеры

	double inv_det = 1.0 / (m->plane_x * m->dir_y - m->dir_x * m->plane_y);
	double transform_x = inv_det * (m->dir_y * sprite_x - m->dir_x * sprite_y);
	double transform_y = inv_det * (-m->plane_y * sprite_x + m->plane_x * sprite_y);

	int	sprite_screen_x = (int) ((WIN_W / 2) * (1 + transform_x / transform_y)); // высота спрайта на экране 

	int	sprite_height = abs((int)(h / transform_y));
	// вычисляем нижный и верхний пиксель
	int draw_start_y = -sprite_height / 2 + h / 2;
	if (draw_start_y < 0)
		draw_start_y = 0;
	int draw_end_y = sprite_height / 2 + h / 2;
	if (draw_end_y >= h)
		draw_end_y = h - 1;
	// вычисляем ширину спрайта 
	int sprite_weight = abs((int)(h / transform_y));
	int draw_start_x = -sprite_weight / 2 + sprite_screen_x;
	if (draw_start_x < 0)
		draw_start_x = 0;
	int draw_end_x = sprite_weight / 2 + sprite_screen_x;
	if (draw_end_x >= WIN_W)
		draw_end_x = WIN_W - 1;
			
	int s = draw_start_x;
	while (s < draw_end_x)
	{
		int tex_x_s = (int)(256 * (s - (-sprite_weight / 2 + sprite_screen_x)) * m->sprite->t_s[0] / sprite_weight) / 256;
		if (transform_y > 0 &&  s > 0 && s < WIN_W && transform_y < m->wall_dist[s])
		{
			int y = draw_start_y;
			while (y < draw_end_y)
			{
				int d = (y) * 256 - h * 128 + sprite_height * 128;
				int tex_y = ((d * m->sprite->t_s[0]) / sprite_height) / 256;
				int color = get_color_tex(tex_x_s, tex_y, m->sprite->addimg[0], m->sprite->t_s[0]);
				if (color != 0x000000)
					draw_tex(s, y, color, m);
				y++;
			}
		}
		s++;
	}
	mlx_put_image_to_window(m->mlx, m->window, m->i->img, 0, 0);
}

int		main(void)
{
	t_mlx	m;
	t_img	img;
	t_sprites	sprite;
	char		world_map[MAP_W][MAP_H] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,3,0,0,0,3,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,4,-1,-1,-1,-1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,-1,-1,-1,5,-1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,4,-1,-1,-1,-1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}	
	};

	if (!(m.map = (char **)malloc(24 * sizeof(char*))))
		exit(0);
	double	pos_x = 6;  // позиция игрока
	double	pos_y = 2;
	double	dir_x = -1;  //начальный вектор направления игрока
	double	dir_y = 0;
	double	plane_x = 0; // плоскость камеры 
	double	plane_y = 0.66; // fov = 66

	m.sing_tex = 0;

	m.pos_x = pos_x;
	m.pos_y = pos_y;
	m.dir_x = dir_x;
	m.dir_y = dir_y;
	m.plane_x = plane_x;
	m.plane_y = plane_y;
	m.mlx = mlx_init();
	m.window = mlx_new_window(m.mlx, WIN_W, WIN_H, "Wolf3d");
	img.img = mlx_new_image(m.mlx, WIN_W, WIN_H);
	img.ptr = mlx_get_data_addr(img.img, &img.bpp, &img.stride, &img.endian);
	img.bpp /= 8;
	m.i = &img;
	img.t_s[0] = 128;
	img.image[0] = mlx_xpm_file_to_image(m.mlx, "./pot.xpm", &img.t_s[0], &img.t_s[0]);
	img.addimg[0] = mlx_get_data_addr(img.image[0], &img.bppimage[0], &img.sl[0], &img.endl[0]);
	img.t_s[1] = 128;
	img.image[1] = mlx_xpm_file_to_image(m.mlx, "./trava.xpm", &img.t_s[1], &img.t_s[1]);
	img.addimg[1] = mlx_get_data_addr(img.image[1], &img.bppimage[1], &img.sl[1], &img.endl[1]);
	img.t_s[2] = 128;
	img.image[2] = mlx_xpm_file_to_image(m.mlx, "./t2.xpm", &img.t_s[2], &img.t_s[2]);
	img.addimg[2] = mlx_get_data_addr(img.image[2], &img.bppimage[2], &img.sl[2], &img.endl[2]);
	img.t_s_w = 700;
	img.t_s_h = 700;
	img.image[3] = mlx_xpm_file_to_image(m.mlx, "./sky3.xpm", &img.t_s_w, &img.t_s_h);
	img.addimg[3] = mlx_get_data_addr(img.image[3], &img.bppimage[3], &img.sl[3], &img.endl[3]);
	img.t_s[4] = 128;
	img.image[4] = mlx_xpm_file_to_image(m.mlx, "./trot128.xpm", &img.t_s[4], &img.t_s[4]);
	img.addimg[4] = mlx_get_data_addr(img.image[4], &img.bppimage[4], &img.sl[4], &img.endl[4]);
	img.t_s[5] = 128;
	img.image[5] = mlx_xpm_file_to_image(m.mlx, "./ser128.xpm", &img.t_s[5], &img.t_s[5]);
	img.addimg[5] = mlx_get_data_addr(img.image[5], &img.bppimage[5], &img.sl[5], &img.endl[5]);
	img.t_s[6] = 128;
	img.image[6] = mlx_xpm_file_to_image(m.mlx, "./kam128.xpm", &img.t_s[6], &img.t_s[6]);
	img.addimg[6] = mlx_get_data_addr(img.image[6], &img.bppimage[6], &img.sl[6], &img.endl[6]);
	

	sprite.t_s[0] = 128;
	sprite.image[0] = mlx_xpm_file_to_image(m.mlx, "./sprite.xpm", &sprite.t_s[0], &sprite.t_s[0]);
	sprite.addimg[0] = mlx_get_data_addr(sprite.image[0], &sprite.bppimage[0], &sprite.sl[0], &sprite.endl[0]);
	sprite.x[0] = 10;
	sprite.y[0] = 10;
	m.sprite = &sprite;


	m.up = 0;


	int i;
	int j;

	i = 0;
	while (i <= 23)
	{
		j = 0;
		if (!(m.map[i] = (char*)malloc(24 * sizeof(char))))
			exit(0);
		while (j <= 23)
		{
			m.map[i][j] = world_map[i][j];
			j++;
		}
		i++;
	}
	/*int x = 0;
	while (x < WIN_W)
	{
		double	camera_x = 2 * x / (double)WIN_W - 1; // координата x на плоскости камеры
		double	ray_dir_x = dir_x + plane_x * camera_x; // направление луча по x
		double	ray_dir_y = dir_y + plane_y * camera_x; // направление луча по y

		int	map_x = (int)pos_x; // в каком квадрате карты мы находимся
		int	map_y = (int)pos_y; // в каком квадрате карты мы находимся

		double side_dist_x; // изначальная длина луча от тек. позиции до следующей
		double side_dist_y; // изначальная длина луча от тек. позиции до следующей

		double delta_dist_x = sqrt(1 + (ray_dir_y * ray_dir_y) / (ray_dir_x * ray_dir_x)); // длина луча от одной стороны x до следующей
		double delta_dist_y = sqrt(1 + (ray_dir_x * ray_dir_x) / (ray_dir_y * ray_dir_y)); // длина луча от одной стороны y до следующей
		double perp_wall_dist; // расчет длины луча до стены 

		int step_x; // движемся по x-направлению (+1)(-1)
		int step_y; // движемся по y-направлению (+1)(-1)

		int hit = 0; // было ли попадание в стену?
		int side; // по какой стороне стены было попадание? по x=0 , по y=1

		// шагаем по x или по y
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - pos_x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_x = (pos_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - pos_y) * delta_dist_y;
		}

		while (hit == 0) // алгоритм DDA
		{
			if (side_dist_x < side_dist_y) // переходим к след. квадрату по xили по y
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if (world_map[map_x][map_y] > 0) // проверяем попал ли луч в стену 
			{
				hit = 1;
			}
		}

		// вычисляем расстояние до стены 
		if (side == 0) // если стена x
		{
			perp_wall_dist = (map_x - pos_x + (1 - step_x) / 2) / ray_dir_x;
		}
		else
		{
			perp_wall_dist = (map_y - pos_y + (1 - step_y) / 2) / ray_dir_y;
		}

		int	h = 600;
		int	wall_height = (int) (h / perp_wall_dist); //вычисляем нижний и верхний пиксель стены
		int	draw_start = -wall_height / 2 + h / 2;
		if (draw_start < 0)
		{
			draw_start = 0;
		}
		int	draw_end = wall_height / 2 + h / 2;
		if (draw_end >= h)
		draw_end = h - 1;

		// выбираем цвет стен 
		int color;
		if (world_map[map_x][map_y] == 1)
		{
			color = 0xf0e68c; // бежевый 
		}
		else
		{
			color = 0xffffff; // white
		}

		if (side == 1) // если сторона y то уменьшаем яркость в 2 раза
		{
			color = color / 2; 
		}
		draw_ver_line(x, draw_start, draw_end, color, &img);
		x++;
	}*/
	draw(&m);
	mlx_hook(m.window, 2, 0, &key_press, (void*)(&m));
	mlx_hook(m.window, 17, 0, &close_win, (void*)(&m));
	mlx_loop(m.mlx);
	return (0);
}