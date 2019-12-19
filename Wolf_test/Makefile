# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aannara <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/06 10:40:08 by aannara           #+#    #+#              #
#    Updated: 2019/11/27 12:08:28 by aannara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = ./src/main.c

OBJ = $(SRC:.c=.o)

FLAGS = -O3

MLX_FLAGS = -I minilibx -lmlx -framework OpenGL -framework AppKit

INC = ./inc

MLX = ./mlx/libmlx.a

NAME = wolf3d

HEADERS = ./inc/wolf.h

all: $(NAME)

$(NAME): $(OBJ) $(MLX) $(HEADERS)
	gcc $(FLAGS) $(MLX_FLAGS) -o $(NAME) $(OBJ) $(MLX)

.c.o: $(HEADERS)
	gcc -I$(INC) $(FLAGS) -c $< -o $@

$(MLX):
	make -C ./mlx/

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	make clean -C ./mlx/
	/bin/rm -f $(NAME)

re: fclean all
