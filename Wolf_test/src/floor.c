/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 16:34:16 by sdiego            #+#    #+#             */
/*   Updated: 2020/02/15 16:34:17 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		floor_w(int x, t_mlx *m)
{
	// пол и потолок
		double floor_x_wall; // положение пола в нижней части стены
		double floor_y_wall; // положение пола в нижней части стены
		
		// 4 различных направления стены
		if (m->wall->side == 0 && m->wall->ray_dir_x > 0)
		{
			floor_x_wall = m->wall->map_x;
			floor_y_wall = m->wall->map_y + m->wall->wall_x;
		}
		else if (m->wall->side == 0 && m->wall->ray_dir_x < 0)
		{
			floor_x_wall = m->wall->map_x + 1;
			floor_y_wall = m->wall->map_y + m->wall->wall_x;
		}
		else if (m->wall->side == 1 && m->wall->ray_dir_y > 0)
		{
			floor_x_wall = m->wall->map_x + m->wall->wall_x;
			floor_y_wall = m->wall->map_y;
		}
		else if (m->wall->side == 1 && m->wall->ray_dir_y < 0)
		{
			floor_x_wall = m->wall->map_x + m->wall->wall_x;
			floor_y_wall = m->wall->map_y + 1;
		}

		double	dist_player; 
		double	current_dis; // текущее расстояние 

		dist_player = 0.0;
		
		/*if (draw_end < 0) // избежания переполнения 
			draw_end = h;
		*/
		int y = m->wall->draw_end;
		while (y < WIN_H)
		{
			current_dis = WIN_H / (2.0 * (y - m->up) - WIN_H);
			double	floor_weight = (current_dis - dist_player) / (m->wall->perp_wall_dist - dist_player);
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

		if (m->wall->side == 0 && m->wall->ray_dir_x > 0)
		{
			floor_x_wall = m->wall->map_x;
			floor_y_wall = m->wall->map_y + m->wall->wall_x;
		}
		else if (m->wall->side == 0 && m->wall->ray_dir_x < 0)
		{
			floor_x_wall = m->wall->map_x + 1;
			floor_y_wall = m->wall->map_y + m->wall->wall_x;
		}
		else if (m->wall->side == 1 && m->wall->ray_dir_y > 0)
		{
			floor_x_wall = m->wall->map_x + m->wall->wall_x;
			floor_y_wall = m->wall->map_y;
		}
		else if (m->wall->side == 1 && m->wall->ray_dir_y < 0)
		{
			floor_x_wall = m->wall->map_x + m->wall->wall_x;
			floor_y_wall = m->wall->map_y + 1;
		}
		y = 0;
		while (y < m->wall->draw_start)
		{
			current_dis = -WIN_H / (2.0 * (y - m->up) - WIN_H);
			double	floor_weight = (current_dis - dist_player) / (m->wall->perp_wall_dist - dist_player);
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
		return (0);
}