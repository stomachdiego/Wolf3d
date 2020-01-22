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

int		get_color_tex(int x, int y, t_mlx *m)
{
	int bpp = m->texture[m->tex_numb]->format->BytesPerPixel;
	Uint32 pixel = *(Uint32 *)(((Uint8*)m->texture[m->tex_numb]->pixels) + bpp * x + m->texture[m->tex_numb]->pitch * y);
	SDL_GetRGB(pixel, m->texture[m->tex_numb]->format, &m->r, &m->g, &m->b);
	return (0);
}


int		sprite_pos_a(t_mlx *m)
{
	int i;
	double sp_dist[m->sprite->sprite_num];
	double pos_x = m->pos_x + m->dir_x * (0.25);
	double pos_y = m->pos_y + m->dir_y * (0.25);

	i = 0;
	
	while (i < m->sprite->sprite_num)
	{
		sp_dist[i] =(pos_x - m->sprite->x[i]) * (pos_x - m->sprite->x[i]) + (pos_y - m->sprite->y[i]) * (pos_y - m->sprite->y[i]);
		if (sp_dist[i] <= 0.075)
			return (0);
		i++;
	}
	return (1);
}

int		sprite_pos_b(t_mlx *m)
{
	int i;
	double sp_dist[m->sprite->sprite_num];
	double pos_x = m->pos_x - m->dir_x * (0.25);
	double pos_y = m->pos_y - m->dir_y * (0.25);

	i = 0;
	
	while (i < m->sprite->sprite_num)
	{
		sp_dist[i] =(pos_x - m->sprite->x[i]) * (pos_x - m->sprite->x[i]) + (pos_y - m->sprite->y[i]) * (pos_y - m->sprite->y[i]);
		if (sp_dist[i] <= 0.075)
			return (0);
		i++;
	}
	return (1);
}

void	wasd(t_mlx *m)
{
	double	speed = 0.05;
	double	speed_move = 0.25;

	if (m->e.key.keysym.sym == SDLK_w)
	{
		if (m->map[(int)(m->pos_x + m->dir_x * (speed_move * 2))][(int)m->pos_y] <= 0 && sprite_pos_a(m) == 1)
			m->pos_x += m->dir_x * speed_move;
		if (m->map[(int)m->pos_x][(int)(m->pos_y + m->dir_y * (speed_move * 2))] <= 0 && sprite_pos_a(m) == 1)
			m->pos_y += m->dir_y * speed_move;
	}
	else if (m->e.key.keysym.sym == SDLK_s)
	{
		if (m->map[(int)(m->pos_x - m->dir_x * (speed_move * 2))][(int)m->pos_y] <= 0 && sprite_pos_b(m) == 1)
			m->pos_x -= m->dir_x * speed_move;
		if (m->map[(int)m->pos_x][(int)(m->pos_y - m->dir_y * (speed_move * 2))] <= 0 && sprite_pos_b(m) == 1)
			m->pos_y -= m->dir_y * speed_move;
	}
	else if (m->e.key.keysym.sym == SDLK_d)
	{
		double	old_dir_x = m->dir_x;
		double	old_plane_x = m->plane_x;

		m->dir_x = m->dir_x * cos(-speed) - m->dir_y * sin(-speed);
		m->dir_y = old_dir_x * sin(-speed) + m->dir_y * cos(-speed);
		m->plane_x = m->plane_x * cos(-speed) - m->plane_y * sin(-speed);
		m->plane_y = old_plane_x * sin(-speed) + m->plane_y * cos(-speed);
		m->sing_tex += 20;
		if (m->sing_tex >= 980)
			m->sing_tex = 0;
	}
	else if (m->e.key.keysym.sym == SDLK_a)
	{
		double	old_dir_x = m->dir_x;
		double	old_plane_x = m->plane_x;

		m->dir_x = m->dir_x * cos(speed) - m->dir_y * sin(speed);
		m->dir_y = old_dir_x * sin(speed) + m->dir_y * cos(speed);
		m->plane_x = m->plane_x * cos(speed) - m->plane_y * sin(speed);
		m->plane_y = old_plane_x * sin(speed) + m->plane_y * cos(speed);
		m->sing_tex -= 20;
		if (m->sing_tex <= 0)
			m->sing_tex = 980;
	}
}


/*int		close_win(void *param)
{
	t_mlx	*m;

	m = (t_mlx*)param;
	exit(0);
}*/

void	up_down(t_mlx *m)
{
	if (m->e.key.keysym.sym == SDLK_UP)
	{
		m->up += 5;
	}
	else if (m->e.key.keysym.sym == SDLK_DOWN)
	{
		m->up -= 5;
	}
}

int		key_press(t_mlx *m)
{
	if (m->e.key.keysym.sym == SDLK_ESCAPE)
		m->run = 1;
	if (m->e.key.keysym.sym == SDLK_w || m->e.key.keysym.sym == SDLK_a || m->e.key.keysym.sym == SDLK_s || m->e.key.keysym.sym == SDLK_d)
		wasd(m);
	else if (m->e.key.keysym.sym == SDLK_UP || m->e.key.keysym.sym == SDLK_DOWN)
		up_down(m);
	//draw(m);
	return (0);
}

/*void	draw_ver_line(int x, int a, int b, int color, t_img *img)
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
}*/

void	clear_map(t_mlx *m)
{
	SDL_SetRenderDrawColor(m->ren, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(m->ren);
}

void	draw(t_mlx *m)
{
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
			if (m->map[map_x][map_y] > 0 && m->map[map_x][map_y] < 5) // проверяем попал ли луч в стену 
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
			draw_end = h;

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
			SDL_SetRenderDrawColor(m->ren, 0xFF, 0xFF, 0xFF, 0xFF);
			if (side == 0 && ray_dir_x > 0)
			{
				m->tex_numb = 0;
				get_color_tex(tex_x, tex_y, m);
				SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
				//color = get_color_tex(tex_x, tex_y, m->i->addimg[2], m->i->t_s[2]);
			}
			if (side == 0 && ray_dir_x < 0)
			{
				m->tex_numb = 1;
				get_color_tex(tex_x, tex_y, m);
				SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
				//SDL_SetRenderDrawColor(m->ren, 0xFF, 0xFF, 0xFF, 0xFF);
				//color = get_color_tex(tex_x, tex_y, m->i->addimg[4], m->i->t_s[4]);
			}	
			if (side == 1 && ray_dir_y > 0)
			{
				m->tex_numb = 2;
				get_color_tex(tex_x, tex_y, m);
				SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
				//SDL_SetRenderDrawColor(m->ren, 0xFF, 0xFF, 0xFF, 0xFF);
				//color = get_color_tex(tex_x, tex_y, m->i->addimg[5], m->i->t_s[5]);
			}	
			if (side == 1 && ray_dir_y < 0)
			{
				m->tex_numb = 3;
				get_color_tex(tex_x, tex_y, m);
				SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
				//SDL_SetRenderDrawColor(m->ren, 0xFF, 0xFF, 0xFF, 0xFF);
				//color = get_color_tex(tex_x, tex_y, m->i->addimg[6], m->i->t_s[6]);
			}
			SDL_RenderDrawPoint(m->ren, x, y);
			//draw_tex(x, y, color, m);
			y++;
		}
		

		
		

		// skybox
		int c = m->sing_tex + x;
		y = 0;
		while (y < draw_start)
		{
			m->tex_numb = 4;
			get_color_tex(c, y + 350 - m->up, m);
			SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
			SDL_RenderDrawPoint(m->ren, x, y);
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
		while (y < h)
		{
			current_dis = h / (2.0 * (y - m->up) - h);
			double	floor_weight = (current_dis - dist_player) / (perp_wall_dist - dist_player);
			double	current_floor_x = floor_weight * floor_x_wall + (1 - floor_weight) * m->pos_x; // координата квадрата на карте
			double	current_floor_y = floor_weight * floor_y_wall + (1 - floor_weight) * m->pos_y; // координата квадрата на карте
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
			
			m->tex_numb = 5;
			get_color_tex(floor_x_tex, floor_y_tex, m);
			SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
			SDL_RenderDrawPoint(m->ren, x, y);
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
		while (y < draw_start)
		{
			current_dis = -h / (2.0 * (y - m->up) - h);
			double	floor_weight = (current_dis - dist_player) / (perp_wall_dist - dist_player);
			double	current_floor_x = floor_weight * floor_x_wall + (1 - floor_weight) * m->pos_x; // координата квадрата на карте
			double	current_floor_y = floor_weight * floor_y_wall + (1 - floor_weight) * m->pos_y; // координата квадрата на карте

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
			
			if ((int)current_floor_x >= MAP_W)
				current_floor_x = MAP_W;
			if ((int)current_floor_y >= MAP_H)
				current_floor_y = MAP_H;
			if ((int)current_floor_x <= 0)
				current_floor_x = 0;
			if ((int)current_floor_y <= 0)
				current_floor_y = 0;
			if (m->map[(int)current_floor_x][(int)current_floor_y] == -1)
			{
				m->tex_numb = 6;
				get_color_tex(floor_x_tex, floor_y_tex, m);
				SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
				SDL_RenderDrawPoint(m->ren, x, y);
			}
			y++;
		}


		


		
		m->wall_dist[x] = perp_wall_dist; // для спрайтов
		x++;
	}
	
	/*
	

	x = 0;
	while (x < WIN_W)
	{
		double	camera_x = 2 * x / (double)WIN_W - 1; // координата x на плоскости камеры
		double	ray_dir_x = m->dir_x + m->plane_x * camera_x; // направление луча по x
		double	ray_dir_y = m->dir_y + m->plane_y * camera_x; // направление луча по y

		//printf("x = %f\n", ray_dir_x);
		//printf("y = %f\n", ray_dir_y);

		double	map_x = m->pos_x; // в каком квадрате карты мы находимся
		double	map_y = m->pos_y; // в каком квадрате карты мы находимся
		
		

		double side_dist_x; // изначальная длина луча от тек. позиции до следующей
		double side_dist_y; // изначальная длина луча от тек. позиции до следующей
		double dist_x = 0;

		double delta_dist_x = sqrt((1 * 1 ) + (ray_dir_y / ray_dir_x) * (ray_dir_y / ray_dir_x)); // длина луча от одной стороны x до следующей
		double delta_dist_y = sqrt((1 * 1 ) + (ray_dir_x / ray_dir_y) * (ray_dir_x / ray_dir_y)); // длина луча от одной стороны y до следующей
		double perp_wall_dist; // расчет длины луча до стены 



		int step_x; // движемся по x-направлению (+1)(-1)
		int step_y; // движемся по y-направлению (+1)(-1)


		int hit = 0; // было ли попадание в стену?
		int side; // по какой стороне стены было попадание? по x=0 , по y=1

		// шагаем по x или по y


		if (ray_dir_x < 0)
		{
			step_x = -1;
			if (m->pos_x - (int)m->pos_x == 0.5)
			{
				dist_x = 0;
				side_dist_x = 0 * delta_dist_x;
			}
			if ((m->pos_x - (int)m->pos_x) < 0.5)
			{
				dist_x = (m->pos_x - (int)map_x + 0.5);
				side_dist_x =  (m->pos_x - (int)map_x + 0.5) * delta_dist_x;
			}
			if ((m->pos_x - (int)m->pos_x) > 0.5)
			{
				dist_x = (m->pos_x - (int)map_x - 0.5);
				side_dist_x = (m->pos_x - (int)map_x - 0.5) * delta_dist_x;
			}
			if (ray_dir_y < 0)
			{
				step_y = -1;
				map_y -= fabs(sqrt((side_dist_x * side_dist_x) - (dist_x * dist_x)));
			}
			else
			{
				step_y = 1;
				map_y += fabs(sqrt((side_dist_x * side_dist_x) - (dist_x * dist_x)));
			}
			map_x -= fabs(dist_x);
		}
		else
		{
			step_x = 1;
			if (m->pos_x - (int)m->pos_x == 0.5)
			{
				dist_x = 0;
				side_dist_x = 0 * delta_dist_x;
			}
			if ((m->pos_x - (int)m->pos_x) < 0.5)
			{
				dist_x = ((int)map_x + 0.5 - m->pos_x);
				side_dist_x = ((int)map_x + 0.5 - m->pos_x) * delta_dist_x;
			}
			if ((m->pos_x - (int)m->pos_x) > 0.5)
			{
				dist_x = ((int)map_x + 1.5 - m->pos_x);
				side_dist_x = ((int)map_x + 1.5 - m->pos_x) * delta_dist_x;
			}
			if (ray_dir_y < 0)
			{
				step_y = -1;
				map_y -= fabs(sqrt((side_dist_x * side_dist_x) - (dist_x * dist_x)));
			}
			else
			{
				step_y = 1;
				map_y += fabs(sqrt((side_dist_x * side_dist_x) - (dist_x * dist_x)));
			}
			map_x += fabs(dist_x);
		}
		
		double map_step_y = sqrt((delta_dist_x * delta_dist_x) - (1 * 1));
		
		if (m->map[(int)map_x][(int)map_y] > 0) // проверяем попал ли луч в стену 
		{
			side = 0;
			hit = 1;
		}
		while (hit == 0) // алгоритм DDA
		{
			map_x += step_x;
			if (step_y > 0)
				map_y += map_step_y;
			else
				map_y -= map_step_y;
			if ((int)map_x < 0 || (int)map_y < 0 || (int)map_x > 24 || (int)map_y > 24)
			{
				hit = 0;
				break ;
			}
			if (m->map[(int)map_x][(int)map_y] > 0) // проверяем попал ли луч в стену 
			{
				side = 0;
				hit = 1;
			}	
		}
		
		


		
		if (m->map[(int)map_x][(int)map_y] == 6 && hit == 1) 
		{	
			
			// вычисляем расстояние до стены 
			if (side == 0) // если стена x
			{
				
				perp_wall_dist = ((int)map_x - m->pos_x + 0.5) / ray_dir_x;
			
				if (m->wall_dist[x] > perp_wall_dist)
				{
					int	h = WIN_H;
					int	wall_height = (int) (h / perp_wall_dist); //вычисляем нижний и верхний пиксель стены
					int	draw_start = (-wall_height / 2 + h / 2) + m->up;
					if (draw_start < 0)
						draw_start = 0;
					int	draw_end = (wall_height / 2 + h / 2) + m->up;
					if (draw_end >= h)
						draw_end = h - 1;
					double wall_x; //где именно было попадание в стену
					if (side == 0)
						wall_x = m->pos_y + perp_wall_dist * ray_dir_y;
					else
						wall_x = m->pos_x + perp_wall_dist * ray_dir_x;
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
						{
							m->tex_numb = 0;
							get_color_tex(tex_x, tex_y, m);
							SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
							//color = get_color_tex(tex_x, tex_y, m->i->addimg[2], m->i->t_s[2]);
							//color = 0xff00ff;
							//draw_tex(x, y, color, m);
						}
						if (side == 0 && ray_dir_x < 0)
						{
							m->tex_numb = 0;
							get_color_tex(tex_x, tex_y, m);
							SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF);
							//color = get_color_tex(tex_x, tex_y, m->i->addimg[4], m->i->t_s[4]);
							//color = 0xffffff;
							//draw_tex(x, y, color, m);
						}
						SDL_RenderDrawPoint(m->ren, x, y);
						//draw_tex(x, y, color, m);
						y++;
					}
					m->wall_dist[x] = perp_wall_dist;
				}
			}
		}
		x++;
	}
	/*
		// sprites
	int i;
	int	temp;
	double temp_d;
	int sort[m->sprite->sprite_num];
	double sprite_dist[m->sprite->sprite_num];
	//  сортировка спрайтов 
	i = 0;
	while (i < m->sprite->sprite_num)
	{
		sprite_dist[i] =(m->pos_x - m->sprite->x[i]) * (m->pos_x - m->sprite->x[i]) + (m->pos_y - m->sprite->y[i]) * (m->pos_y - m->sprite->y[i]);
		sort[i] = i;
		i++;
	}
	i = 1;
	while (i < m->sprite->sprite_num)
	{
		if (sprite_dist[i - 1] < sprite_dist[i])
		{
				temp_d = sprite_dist[i];
				sprite_dist[i] = sprite_dist[i - 1];
				sprite_dist[i - 1] = temp_d;

				temp = sort[i];
				sort[i] = sort[i - 1];
				sort[i - 1] = temp;
				i = 0;
		}
		i++;
	}
	

	i = 0;
	while (i < m->sprite->sprite_num)
	{
		int	x_div = 1; // изменить  ширину спрайта
		int y_div = 1; // изменить  высоту спрайта 
		double y_move = 128.0; // up |  down sprite 
		int	h = WIN_H;
		double sprite_x = m->sprite->x[sort[i]] - m->pos_x; // положение спрайта относительно камеры
		double sprite_y = m->sprite->y[sort[i]] - m->pos_y; // положение спрайта относительно камеры

		double inv_det = 1.0 / (m->plane_x * m->dir_y - m->dir_x * m->plane_y);
		double transform_x = inv_det * (m->dir_y * sprite_x - m->dir_x * sprite_y);
		double transform_y = inv_det * (-m->plane_y * sprite_x + m->plane_x * sprite_y);

		int y_move_screen = (int)(y_move / transform_y) + m->up;


		int	sprite_screen_x = (int) ((WIN_W / 2) * (1 + transform_x / transform_y)); // высота спрайта на экране 

		int	sprite_height = abs((int)(h / transform_y)) / y_div;
		// вычисляем нижный и верхний пиксель
		int draw_start_y = -sprite_height / 2 + h / 2 + y_move_screen;
		if (draw_start_y < 0)
			draw_start_y = 0;
		int draw_end_y = sprite_height / 2 + h / 2 + y_move_screen;
		if (draw_end_y >= h)
			draw_end_y = h - 1;
		// вычисляем ширину спрайта 
		int sprite_weight = abs((int)(h / transform_y)) / x_div;
		int draw_start_x = -sprite_weight / 2 + sprite_screen_x;
		if (draw_start_x < 0)
			draw_start_x = 0;
		int draw_end_x = sprite_weight / 2 + sprite_screen_x;
		if (draw_end_x >= WIN_W)
			draw_end_x = WIN_W - 1;
			
		int s = draw_start_x;
		while (s < draw_end_x)
		{
			int tex_x_s = (int)(256 * (s - (-sprite_weight / 2 + sprite_screen_x)) * m->sprite->t_s[sort[i]] / sprite_weight) / 256;
			if (transform_y > 0 &&  s > 0 && s < WIN_W && transform_y < m->wall_dist[s])
			{
				int y = draw_start_y;
				while (y < draw_end_y)
				{
					int d = (y - y_move_screen) * 256 - h * 128 + sprite_height * 128;
					int tex_y = ((d * m->sprite->t_s[sort[i]]) / sprite_height) / 256;
					int color = get_color_tex(tex_x_s, tex_y, m->sprite->addimg[sort[i]], m->sprite->t_s[sort[i]]);
					if (color != 0x000000)
						draw_tex(s, y, color, m);
					y++;
				}
			}
		s++;
		}
		i++;
	}
*/
	SDL_RenderPresent(m->ren);
}

int		init(t_mlx	*m)
{
	int i;

	i = 1;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("sdl init error : %s\n", SDL_GetError());
		i = 0;
	}

	m->win = SDL_CreateWindow("Wolf3d", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (m->win == NULL)
	{
		printf("sdl create win error : %s\n", SDL_GetError());
		i = 0;
	}
	m->ren = SDL_CreateRenderer(m->win, -1, SDL_RENDERER_SOFTWARE);
	if (m->ren == NULL)
	{
		printf("sdl create ren error : %s\n", SDL_GetError());
		i = 0;
	}
	return (i);
}

void	quit(t_mlx *m)
{
	int i;

	i = 0;
	while (i < m->tn)
	{
		SDL_FreeSurface(m->texture[i]);
		i++;
	}
	SDL_DestroyWindow(m->win);
	m->win = NULL;

	SDL_DestroyRenderer(m->ren);
	m->ren = NULL;

	SDL_Quit();
}

int		load(t_mlx *m)
{
	int i;

	i = 1;
	m->texture[0] = SDL_LoadBMP("./tex/t2.bmp");
	if (m->texture[0] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->texture[1] = SDL_LoadBMP("./tex/kam128.bmp");
	if (m->texture[1] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->texture[2] = SDL_LoadBMP("./tex/ser128.bmp");
	if (m->texture[2] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->texture[3] = SDL_LoadBMP("./tex/trot128.bmp");
	if (m->texture[3] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->texture[4] = SDL_LoadBMP("./tex/sky.bmp");
	if (m->texture[4] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->texture[5] = SDL_LoadBMP("./tex/trava.bmp");
	if (m->texture[5] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->texture[6] = SDL_LoadBMP("./tex/pot.bmp");
	if (m->texture[6] == NULL)
	{
		printf("load error : %s\n", SDL_GetError());
		i = 0;
	}
	m->tn = 7;
	return (i);
}


int		main(void)
{
	t_mlx	m;
	t_img	img;
	t_sprites	sprite;
	char		world_map[MAP_W][MAP_H] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,6,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,3,0,0,0,3,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,1,0,1,1,0,6,0,0,3,6,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,4,-1,-1,-1,-1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,-1,-1,-1,4,-1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,4,-1,-1,-1,-1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}	
	};

	if (!(m.map = (char **)malloc(24 * sizeof(char*))))
		exit(0);
	double	pos_x = 12;  // позиция игрока
	double	pos_y = 16.5;
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
	/*

	sprite.t_s[0] = 128;
	sprite.image[0] = mlx_xpm_file_to_image(m.mlx, "./sprite.xpm", &sprite.t_s[0], &sprite.t_s[0]);
	sprite.addimg[0] = mlx_get_data_addr(sprite.image[0], &sprite.bppimage[0], &sprite.sl[0], &sprite.endl[0]);
	sprite.x[0] = 9.2;
	sprite.y[0] = 10;

	sprite.t_s[1] = 128;
	sprite.image[1] = mlx_xpm_file_to_image(m.mlx, "./sprite.xpm", &sprite.t_s[1], &sprite.t_s[1]);
	sprite.addimg[1] = mlx_get_data_addr(sprite.image[1], &sprite.bppimage[1], &sprite.sl[1], &sprite.endl[1]);
	sprite.x[1] = 12.5;
	sprite.y[1] = 8;

	sprite.t_s[2] = 128;
	sprite.image[2] = mlx_xpm_file_to_image(m.mlx, "./sprite.xpm", &sprite.t_s[2], &sprite.t_s[2]);
	sprite.addimg[2] = mlx_get_data_addr(sprite.image[2], &sprite.bppimage[2], &sprite.sl[2], &sprite.endl[2]);
	sprite.x[2] = 12.0;
	sprite.y[2] = 7;

	sprite.t_s[3] = 128;
	sprite.image[3] = mlx_xpm_file_to_image(m.mlx, "./sprite.xpm", &sprite.t_s[3], &sprite.t_s[3]);
	sprite.addimg[3] = mlx_get_data_addr(sprite.image[3], &sprite.bppimage[3], &sprite.sl[3], &sprite.endl[3]);
	sprite.x[3] = 12.0;
	sprite.y[3] = 10;

	/*sprite.t_s[4] = 128;
	sprite.image[4] = mlx_xpm_file_to_image(m.mlx, "./kam128.xpm", &sprite.t_s[4], &sprite.t_s[4]);
	sprite.addimg[4] = mlx_get_data_addr(sprite.image[4], &sprite.bppimage[4], &sprite.sl[4], &sprite.endl[4]);
	sprite.x[4] = 8;
	sprite.y[4] = 16.5;*/

	sprite.sprite_num = 4;
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
	if (init(&m) == 0)
	{
		quit(&m);
	}
	if (load(&m) == 0)
		quit(&m);
	m.run = 0;
	draw(&m);
	while (m.run == 0)
	{
		while (SDL_PollEvent(&m.e) != 0)
		{
			if (m.e.type == SDL_QUIT)
				m.run = 1;
			if (m.e.type == SDL_KEYDOWN)
				key_press(&m);
		}
		draw(&m);
	}
	quit(&m);
	return (0);
}
