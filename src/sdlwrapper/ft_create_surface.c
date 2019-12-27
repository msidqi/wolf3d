#include "wolf3d.h"

SDL_Surface *ft_create_surface(int win_width, int win_height, int bpp)
{
    Uint32 rmask;
    Uint32 gmask;
    Uint32 bmask;
    Uint32 amask;
    SDL_Surface *bmp;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    amask = 0x000000ff;
    rmask = 0x0000ff00;
    gmask = 0x00ff0000;
    bmask = 0xff000000;
#else
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
#endif
    if (!(bmp = SDL_CreateRGBSurface(0, win_width, win_height, bpp, rmask, gmask, bmask, amask)))
        return (NULL);
    return (bmp);
}