#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "libgl.h"
#define HEIGHT 500
#define WIDTH 500
#define WIN_HEIGHT 720
#define WIN_WIDTH 1280
#define BPP 32

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32 get_pixel32(SDL_Surface *surface, int x, int y);
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Window *ft_sdl_init_create_window(int win_pos_x, int win_pos_y);
SDL_Surface *ft_create_surface(SDL_Window *win, int win_width, int win_height, int bpp);
