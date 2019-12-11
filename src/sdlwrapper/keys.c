#include "wolf3d.h"

int key_code(SDL_Event	event)
{
	return (event.key.keysym.scancode);
}

int scan_code(SDL_Event	event)
{
	return (event.key.keysym.sym);
}

int scan_code_up(SDL_Event	event)
{
	if (event.type == SDL_KEYUP)
		return (event.key.keysym.scancode);
	return (0);
}

int scan_code_down(SDL_Event	event)
{
	if (event.type == SDL_KEYDOWN)
		return (event.key.keysym.scancode);
	return (0);
}