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
	TTF_Quit();
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
		if (walls(x, m, m->wall) != 0)
			return (1);
		if (skybox(x, m) != 0)
			return (1);
		if (floor_w(x, m) != 0)
			return (1);

		m->wall_dist[x] = m->wall->perp_wall_dist; // для спрайтов
		x++;
	}
	if (thinwall_x(m) != 0)
		return (1);
	if (thinwall_y(m) != 0)
		return (1);
	if (sprite(m) != 0)
		return (1);
	
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
	if (TTF_Init() != 0)
	{
		printf("error ttf\n");
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

	/*
	sprite.x[1] = 17;
	sprite.y[1] = 11;

	
	sprite.x[2] = 10;
	sprite.y[2] = 12;

	
	sprite.x[3] = 12.0;
	sprite.y[3] = 10;

	
	sprite.x[4] = 10;
	sprite.y[4] = 18.5;
	*/

	sprite.sprite_num = 1;
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
	
	t_wall	wall;
	m.wall = &wall;

	

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
		if (draw(&m) != 0)
			m.run = 1;
		m.anim += 5;
		if (m.anim > 80)
			m.anim = 0;
	}

	quit(&m);
	return (0);
}
