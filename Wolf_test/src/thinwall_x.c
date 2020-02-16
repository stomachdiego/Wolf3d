/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinwall_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 16:34:40 by sdiego            #+#    #+#             */
/*   Updated: 2020/02/15 16:34:41 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		thinwall_x(t_mlx *m)
{
	int x = 0;
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
	return(0);
}
