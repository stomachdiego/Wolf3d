/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 16:26:14 by sdiego            #+#    #+#             */
/*   Updated: 2020/02/15 16:26:14 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int	walls(int x, t_mlx *m, t_wall *wall)
{
		
		double	camera_x = 2 * x / (double)WIN_W - 1; // координата x на плоскости камеры
		wall->ray_dir_x = m->dir_x + m->plane_x * camera_x; // направление луча по x
		wall->ray_dir_y = m->dir_y + m->plane_y * camera_x; // направление луча по y
		
		wall->map_x = (int)m->pos_x; // в каком квадрате карты мы находимся
		wall->map_y = (int)m->pos_y; // в каком квадрате карты мы находимся
		
	


		double side_dist_x; // изначальная длина луча от тек. позиции до следующей
		double side_dist_y; // изначальная длина луча от тек. позиции до следующей

		double delta_dist_x = sqrt(1 + (wall->ray_dir_y * wall->ray_dir_y) / (wall->ray_dir_x * wall->ray_dir_x)); // длина луча от одной стороны x до следующей
		double delta_dist_y = sqrt(1 + (wall->ray_dir_x * wall->ray_dir_x) / (wall->ray_dir_y * wall->ray_dir_y)); // длина луча от одной стороны y до следующей
		if (delta_dist_y <= -2147483648 || delta_dist_y >= 2147483647)
			delta_dist_y = 0;
		double perp_wall_dist; // расчет длины луча до стены 
		
		int step_x; // движемся по x-направлению (+1)(-1)
		int step_y; // движемся по y-направлению (+1)(-1)

		int hit = 0; // было ли попадание в стену?
		

		// шагаем по x или по y
		if (wall->ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (m->pos_x - wall->map_x) * delta_dist_x;
			
		}
		else
		{
			step_x = 1;
			side_dist_x = (wall->map_x + 1.0 - m->pos_x) * delta_dist_x;
		}
		if (wall->ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (m->pos_y - wall->map_y) * delta_dist_y;
			
		}
		else
		{
			step_y = 1;
			side_dist_y = (wall->map_y + 1.0 - m->pos_y) * delta_dist_y;
		}
		
		while (hit == 0) // алгоритм DDA
		{
			
			if (side_dist_x < side_dist_y) // переходим к след. квадрату по x или по y
			{
				side_dist_x += delta_dist_x;
				wall->map_x += step_x;
				wall->side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				wall->map_y += step_y;
				wall->side = 1;
			}
			if (m->map[wall->map_x][wall->map_y] > 0 && m->map[wall->map_x][wall->map_y] < 5) // проверяем попал ли луч в стену 
			{
				hit = 1;
			}
		}
		

		
		// вычисляем расстояние до стены 
		if (wall->side == 0) // если стена x
		{
			perp_wall_dist = (wall->map_x - m->pos_x + (1 - step_x) / 2) / wall->ray_dir_x;
			
		}
		else
		{
			perp_wall_dist = (wall->map_y - m->pos_y + (1 - step_y) / 2) / wall->ray_dir_y;
		}
		
		int	h = WIN_H;
		int	wall_height = (int) (h / perp_wall_dist); //вычисляем нижний и верхний пиксель стены
		wall->draw_start = (-wall_height / 2 + h / 2) + m->up;
		if (wall->draw_start < 0)
			wall->draw_start = 0;
		wall->draw_end = (wall_height / 2 + h / 2) + m->up;
		if (wall->draw_end >= h)
			wall->draw_end = h;

		
		if (wall->side == 0)
			wall->wall_x = m->pos_y + perp_wall_dist * wall->ray_dir_y;
		else
		{
			wall->wall_x = m->pos_x + perp_wall_dist * wall->ray_dir_x;
		}
		wall->wall_x -= floor((wall->wall_x)); // координата х на текстуре
		int tex_x = (int)(wall->wall_x * (double)TEX_W);
		if (wall->side == 0 && wall->ray_dir_x > 0)
			tex_x = TEX_W - tex_x - 1;
		if (wall->side == 1 && wall->ray_dir_y < 0)
			tex_x = TEX_W - tex_x - 1;
		
		int	y = wall->draw_start;
		int color;
		while (y < wall->draw_end)
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
			
			if (wall->side == 0 && wall->ray_dir_x > 0)
			{
				m->th = 0;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (wall->side == 0 && wall->ray_dir_x < 0)
			{
				m->th = 1;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (wall->side == 1 && wall->ray_dir_y > 0)
			{
				m->th = 2;
				get_color_tex(tex_x, tex_y, m);
				
				if (SDL_SetRenderDrawColor(m->ren, m->r, m->g, m->b, 0xFF) == -1)
				{
					printf("error");
					return(1);
				}
			}
			if (wall->side == 1 && wall->ray_dir_y < 0)
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
		wall->perp_wall_dist = perp_wall_dist;
	return (0);
}