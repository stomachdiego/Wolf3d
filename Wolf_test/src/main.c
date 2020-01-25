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

int		quit(t_mlx *m)
{
	int i;

	i = 0;
	while (i < m->tn)
	{
		SDL_FreeSurface(m->tex[i]);
		i++;
	}
	SDL_DestroyRenderer(m->ren);
	SDL_DestroyWindow(m->win);
	SDL_Quit();
	return(0);
}

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

void	get_color_tex(int x, int y, t_mlx *m)
{
	int bpp = m->tex[m->th]->format->BytesPerPixel;
	Uint32 pixel = *(Uint32 *)(((Uint8*)m->tex[m->th]->pixels) + bpp * x + m->tex[m->th]->pitch * y);
	SDL_GetRGBA(pixel, m->tex[m->th]->format, &m->r, &m->g, &m->b, &m->a);
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


int		close_win(void *param)
{
	t_mlx	*m;

	m = (t_mlx*)param;
	exit(0);
}

void	up_down(t_mlx *m)
{
	if (m->e.key.keysym.sym == SDLK_UP)
	{
		m->up += 5;
		if (m->up > 340)
			m->up = 340;
	}
	else if (m->e.key.keysym.sym == SDLK_DOWN)
	{
		m->up -= 5;
		if (m->up < -370)
			m->up = -370;
	}
}

int		mouse_move(t_mlx *m)
{
	SDL_WarpMouseInWindow(NULL, WIN_W / 2, WIN_H / 2);
	int x = WIN_W / 2;
	int y = WIN_H / 2;

	double speed = abs(m->e.motion.x - WIN_W / 2) * 0.005;
	double speed2 = abs(m->e.motion.y - WIN_H / 2) * 2;
	double speed1 = abs(m->e.motion.x - WIN_W / 2) * 2;
	if (m->e.motion.x > x)
	{
		double	old_dir_x = m->dir_x;
		double	old_plane_x = m->plane_x;

		m->dir_x = m->dir_x * cos(-speed) - m->dir_y * sin(-speed);
		m->dir_y = old_dir_x * sin(-speed) + m->dir_y * cos(-speed);
		m->plane_x = m->plane_x * cos(-speed) - m->plane_y * sin(-speed);
		m->plane_y = old_plane_x * sin(-speed) + m->plane_y * cos(-speed);
		m->sing_tex += speed1;
		if (m->sing_tex >= 980)
			m->sing_tex = 0;
	}
	else if(m->e.motion.x < x)
	{
		double	old_dir_x = m->dir_x;
		double	old_plane_x = m->plane_x;

		m->dir_x = m->dir_x * cos(speed) - m->dir_y * sin(speed);
		m->dir_y = old_dir_x * sin(speed) + m->dir_y * cos(speed);
		m->plane_x = m->plane_x * cos(speed) - m->plane_y * sin(speed);
		m->plane_y = old_plane_x * sin(speed) + m->plane_y * cos(speed);
		m->sing_tex -= speed1;
		if (m->sing_tex <= 0)
			m->sing_tex = 980;
	}
	if (m->e.motion.y > y)
	{
		m->up -= (int)speed2;
		if (m->up < -370)
			m->up = -370;
	}
	else if (m->e.motion.y < y)
	{
		m->up += (int)speed2;
		if (m->up > 340)
			m->up = 340;
		
	}
	/*if (draw(m) != 0)
	{
		m->run = 1;
	}*/
	return (0);
}


int		key_press(t_mlx *m)
{
	if (m->e.key.keysym.sym == SDLK_ESCAPE)
		m->run = 1;
	else if (m->e.key.keysym.sym == SDLK_w || m->e.key.keysym.sym == SDLK_a || m->e.key.keysym.sym == SDLK_s || m->e.key.keysym.sym == SDLK_d)
		wasd(m);
	else if (m->e.key.keysym.sym == SDLK_DOWN || m->e.key.keysym.sym == SDLK_UP)
		up_down(m);
	/*if (draw(m) != 0)
	{
		m->run = 1;
	}*/
	return (0);
}

/*void	draw_ver_line(int x, int a, int b, int color, t_img *img)
{
	int i;
	i = 0;
	
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

int	clear_map(t_mlx *m)
{
	if (SDL_SetRenderDrawColor(m->ren, 0x00, 0x00, 0x00, 0x00) == -1)
	{
		printf("error");
		return (1);
	}
	if (SDL_RenderClear(m->ren) == -1)
	{
		printf("error");
		return (1);
	}
	return (0);
}

int	draw(t_mlx *m)
{
	if (clear_map(m) != 0)
	{
		return (1);
		m->run = 1;
	}
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
		if (delta_dist_y <= -2147483648 || delta_dist_y >= 2147483647)
			delta_dist_y = 0;
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
			
			if (side == 0 && ray_dir_x > 0)
			{
				m->th = 0;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (side == 0 && ray_dir_x < 0)
			{
				m->th = 1;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (side == 1 && ray_dir_y > 0)
			{
				m->th = 2;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (side == 1 && ray_dir_y < 0)
			{
				m->th = 3;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (SDL_RenderDrawPoint(m->ren, x, y) == -1)
			{
				printf("error");
				return(1);
			}
			y++;
		}
		


		

		// skybox
		int c = m->sing_tex + x;
		y = 0;
		while (y < draw_start)
		{
			m->th = 4;
			get_color_tex(c, y + 350 - m->up, m);
			if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
			{
				printf("error");
				return(1);
			}
			if (SDL_RenderDrawPoint(m->ren, x, y) == -1)
			{
				printf("error");
				return(1);
			}
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

			m->th = 5;
			get_color_tex(floor_x_tex, floor_y_tex, m);
			if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
			{
				printf("error");
				return(1);
			}
			if (SDL_RenderDrawPoint(m->ren, x, y) == -1)
			{
				printf("error");
				return(1);
			}
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
				m->th = 6;
				get_color_tex(floor_x_tex, floor_y_tex, m);
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
				{
					printf("error");
					return(1);
				}
				if (SDL_RenderDrawPoint(m->ren, x, y) == -1)
				{
					printf("error");
					return(1);
				}
			}
			y++;
		}


		



		m->wall_dist[x] = perp_wall_dist; // для спрайтов
		x++;
	}
	
	


	x = 0;
	while (x < WIN_W)
	{
		double	camera_x = 2 * x / (double)WIN_W - 1; // координата x на плоскости камеры
		double	ray_dir_x = m->dir_x + m->plane_x * camera_x; // направление луча по x
		double	ray_dir_y = m->dir_y + m->plane_y * camera_x; // направление луча по y

		

		double	map_x = m->pos_x; // в каком квадрате карты мы находимся
		double	map_y = m->pos_y; // в каком квадрате карты мы находимся
		
		

		double side_dist_x; // изначальная длина луча от тек. позиции до следующей
		double side_dist_y; // изначальная длина луча от тек. позиции до следующей
		double dist_x = 0;

		double delta_dist_x = sqrt((1 * 1 ) + (ray_dir_y / ray_dir_x) * (ray_dir_y / ray_dir_x)); // длина луча от одной стороны x до следующей
		
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
				map_y -= sqrt(fabs((side_dist_x * side_dist_x) - (dist_x * dist_x)));
				
			}
			else
			{
				step_y = 1;
				map_y += sqrt(fabs((side_dist_x * side_dist_x) - (dist_x * dist_x)));
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
				map_y -= sqrt(fabs((side_dist_x * side_dist_x) - (dist_x * dist_x)));
			}
			else
			{
				step_y = 1;
				map_y += sqrt(fabs((side_dist_x * side_dist_x) - (dist_x * dist_x)));
			}
			map_x += fabs(dist_x);
		}
		
		double map_step_y = sqrt(fabs((delta_dist_x * delta_dist_x) - (1 * 1)));
		if (map_y < 0)
		{
			map_y = map_y - (int)map_y;
			map_y = fabs(map_y);
		}
		if (map_y > 23)
		{
			map_y = map_y - (int)map_y + 23;
		}
		
		if ((int)map_y > 0 && (int)map_y <= 23)
		{
			if  (m->map[(int)map_x][(int)map_y] == 6) // проверяем попал ли луч в стену 
			{
				side = 0;
				hit = 1;
			}
		}
		while (hit == 0) // алгоритм DDA
		{
			map_x += step_x;
			if (step_y > 0)
				map_y += map_step_y;
			else
				map_y -= map_step_y;
			if ((int)map_x < 0 || (int)map_y < 0 || (int)map_x > 23 || (int)map_y > 23)
			{
				break ;
			}
			if (m->map[(int)map_x][(int)map_y] == 6) // проверяем попал ли луч в стену 
			{
				side = 0;
				hit = 1;
			}	
		}
		
		if (hit == 1 && m->map[(int)map_x][(int)map_y] == 6)
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
						draw_end = h;
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
							m->th = 0;
							get_color_tex(tex_x, tex_y, m);
							if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
							{
								printf("error");
								return(1);
							}
						}
						if (side == 0 && ray_dir_x < 0)
						{
							m->th = 0;
							get_color_tex(tex_x, tex_y, m);
							if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
							{
								printf("error");
								return(1);
							}
						}
						
						if (SDL_RenderDrawPoint(m->ren, x, y) == -1)
						{
							printf("error");
							return(1);
						}
						y++;
					}
					m->wall_dist[x] = perp_wall_dist;
				}
			}
		}
		x++;
	}

	
	// для тонкой  стены по y

	x = 0;
	while (x < WIN_W)
	{
		double	camera_x = 2 * x / (double)WIN_W - 1; // координата x на плоскости камеры
		double	ray_dir_x = m->dir_x + m->plane_x * camera_x; // направление луча по x
		double	ray_dir_y = m->dir_y + m->plane_y * camera_x; // направление луча по y

		double	map_x = m->pos_x; // в каком квадрате карты мы находимся
		double	map_y = m->pos_y; // в каком квадрате карты мы находимся
		
		

		double side_dist_x; // изначальная длина луча от тек. позиции до следующей
		double side_dist_y; // изначальная длина луча от тек. позиции до следующей
		double dist_y = 0;

		
		double delta_dist_y = sqrt((1 * 1 ) + (ray_dir_x / ray_dir_y) * (ray_dir_x / ray_dir_y)); // длина луча от одной стороны y до следующей
		double perp_wall_dist; // расчет длины луча до стены 
		
		if (delta_dist_y <= -2147483648 || delta_dist_y >= 2147483647)
			delta_dist_y = 0;
		

		int step_x; // движемся по x-направлению (+1)(-1)
		int step_y; // движемся по y-направлению (+1)(-1)


		int hit = 0; // было ли попадание в стену?
		int side; // по какой стороне стены было попадание? по x=0 , по y=1

		// шагаем по x или по y


		if (ray_dir_y < 0)
		{
			step_y = -1;
			if (m->pos_y - (int)m->pos_y == 0.5)
			{
				dist_y = 0;
				side_dist_y = 0;
			}
			if ((m->pos_y - (int)m->pos_y) < 0.5)
			{
				dist_y = (m->pos_y - (int)map_y + 0.5);
				side_dist_y =  (m->pos_y - (int)map_y + 0.5) * delta_dist_y;
			}
			if ((m->pos_y - (int)m->pos_y) > 0.5)
			{
				dist_y = (m->pos_y - (int)map_y - 0.5);
				side_dist_y = (m->pos_y - (int)map_y - 0.5) * delta_dist_y;
			}
			if (ray_dir_x < 0)
			{
				step_x = -1;
				map_x -= sqrt(fabs((side_dist_y * side_dist_y) - (dist_y * dist_y)));
				
			}
			else
			{
				step_x = 1;
				map_x += sqrt(fabs((side_dist_y * side_dist_y) - (dist_y * dist_y)));
			}
			map_y -= fabs(dist_y);
		}
		else
		{
			step_y = 1;
			if (m->pos_y - (int)m->pos_y == 0.5)
			{
				dist_y = 0;
				side_dist_y = 0;
			}
			if ((m->pos_y - (int)m->pos_y) < 0.5)
			{
				dist_y = ((int)map_y + 0.5 - m->pos_y);
				side_dist_y = ((int)map_y + 0.5 - m->pos_y) * delta_dist_y;
			}
			if ((m->pos_y - (int)m->pos_y) > 0.5)
			{
				dist_y = ((int)map_y + 1.5 - m->pos_y);
				side_dist_y = ((int)map_y + 1.5 - m->pos_y) * delta_dist_y;
			}
			if (ray_dir_x < 0)
			{
				step_x = -1;
				map_x -= sqrt(fabs((side_dist_y * side_dist_y) - (dist_y * dist_y)));
			}
			else
			{
				step_x = 1;
				map_x += sqrt(fabs((side_dist_y * side_dist_y) - (dist_y * dist_y)));
			}
			map_y += fabs(dist_y);
		}
		if (delta_dist_y == 0)
		{
			double map_step_x = 0;
		}

		double map_step_x = sqrt(fabs((delta_dist_y * delta_dist_y) - (1 * 1)));

		if (map_x < 0)
		{
			map_x = map_x - (int)map_x;
			map_x = fabs(map_x);
		}
		if (map_x > 23)
		{
			map_x = map_x - (int)map_x + 23;
		}
		if ((int)map_x > 0 && (int)map_x <= 23)
		{
			if (m->map[(int)map_x][(int)map_y] == 7) // проверяем попал ли луч в стену 
			{
				side = 1;
				hit = 1;
			}
		}
		
		while (hit == 0) // алгоритм DDA
		{
			
			map_y += step_y;
			if (step_x > 0)
				map_x += map_step_x;
			else
				map_x -= map_step_x;
			if ((int)map_x < 0 || (int)map_y < 0 || (int)map_x > 23 || (int)map_y > 23)
			{
				break ;
			}
			if (m->map[(int)map_x][(int)map_y] == 7) // проверяем попал ли луч в стену 
			{
				side = 1;
				hit = 1;
			}
			
		}
		if (hit == 1 && m->map[(int)map_x][(int)map_y] == 7)
		{	
			// вычисляем расстояние до стены 
			if (side == 1) // если стена x
			{
				
				perp_wall_dist = ((int)map_y - m->pos_y + 0.5) / ray_dir_y;
			
				if (m->wall_dist[x] > perp_wall_dist)
				{
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
			
						if (side == 1 && ray_dir_y > 0)
						{
							m->th = 0;
							get_color_tex(tex_x, tex_y, m);
							if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
							{
								printf("error");
								return(1);
							}
						}
						if (side == 1 && ray_dir_y < 0)
						{
							m->th = 0;
							get_color_tex(tex_x, tex_y, m);
							if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
							{
								printf("error");
								return(1);
							}
						}
						
						if (SDL_RenderDrawPoint(m->ren, x, y) == -1)
						{
							printf("error");
							return(1);
						}
						y++;
					}
					m->wall_dist[x] = perp_wall_dist;
				}
			}
		}
		x++;
	}






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
		
		
		
		
		
		// anim
		if (sort[i] == 1)
			m->th = 8;
		else
			m->th = 7;
		// anim



		
		int s = draw_start_x;
		while (s < draw_end_x)
		{
			int tex_x_s = (int)(256 * (s - (-sprite_weight / 2 + sprite_screen_x)) * 128 / sprite_weight) / 256;
			if (transform_y > 0 &&  s > 0 && s < WIN_W && transform_y < m->wall_dist[s])
			{
				// anim
				if (m->th == 8)
					{
						if (m->anim >= 10 && m->anim <= 20)
							tex_x_s += 128;
						if (m->anim >= 21 && m->anim <= 30)
							tex_x_s += 256;
						if (m->anim >= 31 && m->anim <= 40)
							tex_x_s += 384;
						
						if (m->anim >= 51 && m->anim <= 60)
						{
							tex_x_s += 128;
						}
						if (m->anim >= 61 && m->anim <= 70)
						{
							tex_x_s += 256;
						}
						if (m->anim >= 71 && m->anim <= 80)
						{
							tex_x_s += 384;
						}
					}
				// anim



				int y = draw_start_y;
				while (y < draw_end_y)
				{
					int d = (y - y_move_screen) * 256 - h * 128 + sprite_height * 128;
					int tex_y = ((d * 128) / sprite_height) / 256;
					
					
					
					
					
					// anim
					
					if (m->th == 8)
					{
						
						if (m->anim >= 41 && m->anim <= 50)
							tex_y += 128;
						if (m->anim >= 51 && m->anim <= 60)
						{
							
							tex_y += 128;
						}
						if (m->anim >= 61 && m->anim <= 70)
						{
							tex_y += 128;
							
						}
						if (m->anim >= 71 && m->anim <= 80)
						{
							tex_y += 128;
							
						}
					}

					// anim









					get_color_tex(tex_x_s, tex_y, m);
					if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xff) == -1)
					{
						printf("error");
						return(1);
					}
					if (m->a != 0)
					{
						if (SDL_RenderDrawPoint(m->ren, s, y) == -1)
						{
							printf("error");
							return(1);
						}
					}
					y++;
				}
			}
			s++;
		}
		i++;
	}




	SDL_RenderPresent(m->ren);
	return (0);
	
}


int		init(t_mlx *m)
{
	int i;

	i = 0;
	/*m->win = NULL;
	m->tex[0] = NULL;
	m->ren = NULL;*/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->win = SDL_CreateWindow("Wolf3d", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (m->win == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->ren = SDL_CreateRenderer(m->win, -1, SDL_RENDERER_SOFTWARE);
	if (m->ren == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	return(i);
}

int		load(t_mlx *m)
{
	int	i;

	i = 0;
	m->tex[0] = SDL_LoadBMP("./tex/t2.bmp");
	if (m->tex[0] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[1] = SDL_LoadBMP("./tex/kam128.bmp");
	if (m->tex[1] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[2] = SDL_LoadBMP("./tex/ser128.bmp");
	if (m->tex[2] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[3] = SDL_LoadBMP("./tex/trot128.bmp");
	if (m->tex[3] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[4] = SDL_LoadBMP("./tex/sky.bmp");
	if (m->tex[4] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[5] = SDL_LoadBMP("./tex/trava.bmp");
	if (m->tex[5] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[6] = SDL_LoadBMP("./tex/pot.bmp");
	if (m->tex[6] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[7] = SDL_LoadBMP("./tex/sprite.bmp");
	if (m->tex[7] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tex[8] = SDL_LoadBMP("./tex/animPJ.bmp");
	if (m->tex[8] == NULL)
	{
		printf("%s\n", SDL_GetError());
		i = 1;
	}
	m->tn = 9;
	return(i);
}



int		main(void)
{
	t_mlx	m;
	t_img	img;
	t_sprites	sprite;
	char		world_map[MAP_W][MAP_H] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,6,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,1},
  		{1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,3,0,3,0,3,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,7,0,0,0,0,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,7,0,0,3,0,0,0,3,0,0,0,1},
  		{1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,7,0,0,0,1},
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
  		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,6,1},
  		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}	
	};

	if (init(&m) != 0)
		quit(&m);
	if (load(&m) != 0)
		quit(&m);


	if (!(m.map = (char **)malloc(24 * sizeof(char*))))
		exit(0);
	double	pos_x = 12;  // позиция игрока
	double	pos_y = 16;
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
	
	sprite.x[0] = 10;
	sprite.y[0] = 10;

	
	sprite.x[1] = 10;
	sprite.y[1] = 11;

	
	sprite.x[2] = 10;
	sprite.y[2] = 12;

	
	sprite.x[3] = 12.0;
	sprite.y[3] = 10;

	
	sprite.x[4] = 10;
	sprite.y[4] = 18.5;

	sprite.sprite_num = 5;
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
	
	if (draw(&m) != 0)
		quit(&m);

	SDL_ShowCursor(SDL_DISABLE);
	
	m.anim = 0;
	m.run = 0;
	while (m.run == 0)
	{
		while (SDL_PollEvent(&m.e) != 0)
		{
			if (m.e.type == SDL_QUIT)
				m.run = 1;
			if (m.e.type == SDL_KEYDOWN)
				key_press(&m);
			if (m.e.type == SDL_MOUSEMOTION)
				mouse_move(&m);
		}
		draw(&m);
		m.anim += 5;
		if (m.anim > 80)
			m.anim = 0;
	}

	quit(&m);
	return (0);
}
