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
LIBGL_PATH = src/libgl/
LIBGLL = src/libgl/src/libgl.a
SRC_PATH = src
GLSRC_PATH = src/libgl
WRAP_SRC = src/sdlwrapper/*.c
INCLUDE = include
INC = include/libgl.h
OBJS_DIR = .objs
CC = gcc -Wall -Wextra -Werror
OBJ = $(addprefix $(OBJS_DIR)/,$(WRAP_SRC:.c=.o))

all : $(NAME)

$(OBJ) : $(OBJS_DIR)/%.o : $(WRAP_SRC)/%.c $(INC) | $(OBJS_DIR)
	$(CC) -c $< -o $@ -I$(INCLUDE)
$(LIBGLL) :
	make -C $(LIBGL_PATH)
$(NAME) : $(WRAP_SRC) main.c $(LIBGLL)
	$(CC) $^ -o $@ -I$(INCLUDE) $(LIBSDL) $(LIBSDL_ttf) $(LIBGLL)

clean :
	make fclean -C $(LIBGL_PATH)
	rm -rf $(OBJ) 
#$(LIBGLL)   gcc main.c src/sdlwrapper/*.c -o wolf -I include  libSDL/libSDL2-2.0.0.dylib
fclean : clean
	rm -rf $(NAME)
re : fclean all

$(OBJS_DIR):
	mkdir $(OBJS_DIR)
