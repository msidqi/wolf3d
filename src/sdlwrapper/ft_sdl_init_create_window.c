#include "wolf3d.h"

SDL_Window *ft_sdl_init_create_window(int win_pos_x, int win_pos_y, int win_width, int win_height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_Window *win = SDL_CreateWindow("call of duty modern warfare", win_pos_x, win_pos_y, win_width, win_height, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
    return (win);
}
