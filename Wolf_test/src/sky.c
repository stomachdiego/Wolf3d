/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sky.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdiego <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 16:25:52 by sdiego            #+#    #+#             */
/*   Updated: 2020/02/15 16:25:53 by sdiego           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		skybox(int x, t_mlx *m)
{

	int c = m->sing_tex + x;
	int y = 0;
	while (y < m->wall->draw_start)
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
	return (0);
}