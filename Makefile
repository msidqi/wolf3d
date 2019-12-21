# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msidqi <msidqi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/23 00:08:25 by msidqi            #+#    #+#              #
#    Updated: 2019/10/11 12:15:24 by msidqi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME2 = wolf3d
NAME = callOfDuty_ModernWarfare
LIBSDL = src/libSDL/libSDL2-2.0.0.dylib
LIBSDL_ttf = src/libSDL_ttf/libSDL2_ttf-2.0.0.dylib
LIBSDL_image = src/libSDL_image/libSDL2_image.dylib 
LIBGL_PATH = src/libgl/
LIBGLL = src/libgl/src/libgl.a
SRC_PATH = src
GLSRC_PATH = src/libgl
WRAP_SRC = src/sdlwrapper/*.c
INCLUDE = include
INC = include/libgl.h
WOLF_H = include/wolf3d.h
OBJS_DIR = .objs
CC = gcc -Wall -Wextra -Werror
OBJ = $(addprefix $(OBJS_DIR)/,$(WRAP_SRC:.c=.o))

all : $(NAME)

$(OBJ) : $(OBJS_DIR)/%.o : $(WRAP_SRC)/%.c $(WOLF_H) | $(OBJS_DIR)
	$(CC) -c $< -o $@ -I$(INCLUDE)
$(LIBGLL) :
	make -C $(LIBGL_PATH)
$(NAME) : main.c $(WRAP_SRC) $(LIBGLL) $(WOLF_H)
	$(CC) -o $@ $< $(WRAP_SRC) -I$(INCLUDE) $(LIBSDL) $(LIBSDL_ttf) $(LIBSDL_image) $(LIBGLL)

clean :
	make fclean -C $(LIBGL_PATH)
	rm -rf $(OBJ) 
#$(LIBGLL)   gcc main.c src/sdlwrapper/*.c -o wolf -I include  libSDL/libSDL2-2.0.0.dylib
fclean : clean
	rm -rf $(NAME)
re : fclean all

$(OBJS_DIR):
	mkdir $(OBJS_DIR)
