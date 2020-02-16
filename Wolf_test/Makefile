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

SRC = ./src/main.c ./src/wall.c ./src/sky.c ./src/floor.c ./src/thinwall_x.c ./src/thinwall_y.c ./src/sprite.c 

OBJ = $(SRC:.c=.o)

FLAGS = -O3

SDL_FLAGS = -I include -I SDL_lib/SDL2.framework/Headers -I SDL_lib/SDL2_image.framework/Headers -I SDL_lib/SDL2_ttf.framework/Headers

SDL2 = -framework SDL2 -framework SDL2_image -framework SDL2_ttf  -F ./SDL_lib/

SDL2_LOAD = -rpath @loader_path/SDL_lib/

INC = ./inc

NAME = wolf3d

HEADERS = ./inc/wolf.h

all: $(NAME)

$(NAME): $(OBJ) $(MLX) $(HEADERS)
	gcc -o $(NAME) $(OBJ) $(FLAGS) $(SDL2_LOAD) $(SDL2)

.c.o: $(HEADERS)
	gcc -I$(INC) $(FLAGS) $(SDL_FLAGS) -c $< -o $@

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
