#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "libgl.h"
#define HEIGHT 500
#define WIDTH 500
#define WIN_HEIGHT 720
#define WIN_WIDTH 1280
#define BPP 32
#define DOWN (t_vec3){0, 1, 0}
#define UP (t_vec3){0, -1, 0}
#define LEFT (t_vec3){-1, 0, 0}
#define RIGHT (t_vec3){1, 0, 0}
#define FORW (t_vec3){0, 0, -1}
#define BACK (t_vec3){0, 0, 1}
#define TILE_WIDTH 100
#define TILE_HEIGHT 100

typedef struct s_action
{
	int			type;
}				t_action;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			dir;
	double			t;
	double			angle;
	t_vec3			first_inter_point;
	t_vec3			increments;
	t_vec3			wall_inter;
}					t_ray;

typedef t_vec2int t_index;
typedef struct s_player
{
	t_vec3		pos;
	t_vec3		forw;
    t_vec3      right;
	double		focal_len;
	double		cam_hor;
	double		cam_ver;
	// double		height;
    // t_camera    camera;
}				t_player;

typedef struct s_texture
{
	SDL_Surface	pixels;
	double		height;
}				t_texture;

typedef struct s_tile
{
	t_vec3int	pos;
	double		height;
	double		width;
	double		depth;
	t_texture	ground;
	t_texture	wall;
}				t_tile;

typedef struct s_map
{
	int			height;
	int			width;
	int			skybox;
	t_tile		**tiles;
}				t_map;

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32 get_pixel32(SDL_Surface *surface, int x, int y);
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Window *ft_sdl_init_create_window(int win_pos_x, int win_pos_y, int win_width, int win_height);
SDL_Surface *ft_create_surface(SDL_Window *win, int win_width, int win_height, int bpp);
int key_code(SDL_Event	event);
int scan_code(SDL_Event	event);
int scan_code_up(SDL_Event	event);
int scan_code_down(SDL_Event	event);
void ft_print_map(t_map *map, t_player *player);
void ft_draw_map(SDL_Surface *surface, t_map *map, t_player *player);
t_map	*ft_create_map(int width, int height);
void	ft_destroy_map(t_map *map);