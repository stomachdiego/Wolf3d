/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:06:57 by sdiego            #+#    #+#             */
/*   Updated: 2020/02/16 13:06:59 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		sprite(t_mlx *m)
{
		// sprites
	int i;
	int	temp;
	double temp_d;
	int sort[m->sprite->sprite_num];
	double sprite_dist[m->sprite->sprite_num];
	//  сортировка спрайтов 
	i = 0;
	m->check_anim = -1;
	while (i < m->sprite->sprite_num)
	{
		sprite_dist[i] =(m->pos_x - m->sprite->x[i]) * (m->pos_x - m->sprite->x[i]) + (m->pos_y - m->sprite->y[i]) * (m->pos_y - m->sprite->y[i]);
		sort[i] = i;
	//	printf("%f\n", sprite_dist[i]);
	/*	if (sprite_dist[i] < 35)
		{
			sprite_move(i, m, sprite_dist[i]);
		}*/
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
		if (sprite_dist[i] < 35)
		{
			sprite_move(sort[i], m, sprite_dist[i]);
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
		
		if (m->check_anim == 1)
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
	return(0);
}

int		sprite_move(int i, t_mlx *m, double sprite_dist)
{
	int check;

	check = 0;
	if (sprite_dist > 0.5)
	{
		if (m->sprite->x[i] < (int)m->pos_x)
		{
			if (m->map[(int)(m->sprite->x[i] + 0.5)][(int)m->sprite->y[i]] <= 0 && check_sprite_p(m, i) == 0)
			{
				m->sprite->x[i] += 0.035;
				m->check_anim = 1;
				check = 1;
			}
				
		}
		if (m->sprite->x[i] > (int)m->pos_x)
		{
			if (m->map[(int)(m->sprite->x[i] - 0.5)][(int)m->sprite->y[i]] <= 0 && check_sprite_n(m, i) == 0)
			{
				m->sprite->x[i] -= 0.035;
				m->check_anim = 1;
				check = 1;
			}

		}
		if (m->sprite->y[i] < (int)m->pos_y)
		{
			if (m->map[(int)m->sprite->x[i]][(int)(m->sprite->y[i] + 0.5)] <= 0 && check_sprite_p(m, i) == 0)
			{
				m->sprite->y[i] += 0.035;
				m->check_anim = 1;
				check = 1;
			}
				
		}
		if (m->sprite->y[i] > (int)m->pos_y)
		{
			if (m->map[(int)m->sprite->x[i]][(int)(m->sprite->y[i] - 0.5)] <= 0 && check_sprite_n(m, i) == 0)
			{
				m->sprite->y[i] -= 0.035;
				m->check_anim = 1;
				check = 1;
			}	
		}
		if (check == 0)
		{
			m->check_anim = -1;
		}
		check = 0;
		
	}
	return (0);
}

int		check_sprite_p(t_mlx *m, int i)
{
	int a;
	a = 0;
	while (a < m->sprite->sprite_num)
	{
		if (a == i)
		{
			a++;
		}
		if ((int)(m->sprite->x[i] + 0.5) == (int)m->sprite->x[a])
		{
			if ((int)(m->sprite->y[i]) == (int)m->sprite->y[a])
			{
				return (1);
			}
		}
		if ((int)(m->sprite->y[i] + 0.5) == (int)m->sprite->y[a])
		{
			if ((int)(m->sprite->x[i]) == (int)m->sprite->x[a])
			{
				return (1);
			}
		}
		a++;
	}
	return (0);
}

int		check_sprite_n(t_mlx *m, int i)
{
	int a;
	a = 0;
	while (a < m->sprite->sprite_num)
	{
		if (a == i)
		{
			a++;
		}
		if ((int)(m->sprite->x[i] - 0.5) == (int)m->sprite->x[a])
		{
			if ((int)(m->sprite->y[i]) == (int)m->sprite->y[a])
			{
				return (1);
			}
		}
		if ((int)(m->sprite->y[i] - 0.5) == (int)m->sprite->y[a])
		{
			if ((int)(m->sprite->x[i]) == (int)m->sprite->x[a])
			{
				return (1);
			}
		}
		a++;
	}
	return (0);
}