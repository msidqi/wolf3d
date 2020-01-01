# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aabouibr <aabouibr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/23 00:08:25 by msidqi            #+#    #+#              #
#    Updated: 2019/12/31 17:26:21 by aabouibr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
LIBSDL = src/libSDL/libSDL2-2.0.0.dylib
LIBSDL_ttf = src/libSDL_ttf/libSDL2_ttf-2.0.0.dylib
LIBSDL_image = src/libSDL_image/libSDL2_image-2.0.0.dylib
LIBSDL_mixer = src/libSDL_mixer/libSDL2_mixer-2.0.0.dylib
LIBGL_PATH = src/libgl/
LIBGLL = $(LIBGL_PATH)src/libgl.a
LIBFT_PATH = libft/
LIBFTL = $(LIBFT_PATH)libft.a
SRC_PATH = src
GLSRC_PATH = src/libgl
WRAP_SRC1 = ft_clean_resources.c           ft_parser.c \
ft_create_surface.c            ft_ray_cast.c \
ft_find_closest_wall.c         ft_screenshot.c \
ft_find_closest_wall_helpers.c ft_sdl_init_create_window.c \
ft_game_engine.c               ft_texture.c \
ft_inter_tools.c               keys.c \
ft_map.c                       player.c \
ft_menu.c                      player_controller.c \
ft_menu_tools.c                player_physics.c \
ft_mini_map_func.c             putpixel.c \
ft_parse_tools.c
WRAP_PATH = src/sdlwrapper
WRAP_SRC = $(addprefix $(WRAP_PATH)/,$(WRAP_SRC1))
INCLUDE = include
INC = include/libgl.h
WOLF_H = include/wolf3d.h
OBJS_DIR = .objs
CC = gcc -Wall -Wextra -Werror
OBJ = $(addprefix $(OBJS_DIR)/,$(WRAP_SRC:.c=.o))

all : $(NAME)

$(OBJ) : $(OBJS_DIR)/%.o : $(WRAP_PATH)/%.c $(WOLF_H) | $(OBJS_DIR)
	$(CC) -c $< -o $@ -I$(INCLUDE)
$(LIBGLL) :
	make -C $(LIBGL_PATH)
$(LIBFTL) :
	make -C $(LIBFT_PATH)
$(NAME) : main.c $(WRAP_SRC) $(LIBGLL) $(LIBFTL) $(WOLF_H)
	$(CC) -o $@ $< $(WRAP_SRC) -I$(INCLUDE) $(LIBSDL) $(LIBSDL_mixer) $(LIBSDL_ttf) $(LIBSDL_image) $(LIBGLL) $(LIBFTL)

clean :
	make fclean -C $(LIBGL_PATH)
	make fclean -C $(LIBFT_PATH)
	rm -rf $(OBJ) 
#$(LIBGLL)   gcc main.c src/sdlwrapper/*.c -o wolf -I include  libSDL/libSDL2-2.0.0.dylib
fclean : clean
	rm -rf $(NAME)
re : fclean all

$(OBJS_DIR):
	mkdir $(OBJS_DIR)
