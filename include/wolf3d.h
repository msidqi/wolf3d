#ifndef WOLF3D_H
#define WOLF3D_H

# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_ttf.h"
# include "SDL2/SDL_timer.h"
# include "SDL2/SDL_mixer.h"
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include "libgl.h"
# include "libft.h"
# define HEIGHT 500
# define WIDTH 500
# define WIN_HEIGHT 720
# define WIN_WIDTH 1280
# define BPP 32
# define DOWN (t_vec3){0, 1, 0}
# define UP (t_vec3){0, -1, 0}
# define LEFT (t_vec3){-1, 0, 0}
# define RIGHT (t_vec3){1, 0, 0}
# define FORW (t_vec3){0, 0, -1}
# define BACK (t_vec3){0, 0, 1}
# define TILE_WIDTH 1
# define TILE_HEIGHT 1
# define TILE_DEPTH 1
# define MINI_MAP_TILE_WIDTH 10
# define MINI_MAP_TILE_HEIGHT 10
# define MINI_MAP_RATIO_WIDTH ((double)TILE_WIDTH / (double)MINI_MAP_TILE_WIDTH)
# define MINI_MAP_RATIO_HEIGHT ((double)TILE_HEIGHT / (double)MINI_MAP_TILE_HEIGHT)
# define BMP_WIDTH 1280
# define BMP_HEIGHT 720
# define MINI_MAP_BMP_HEIGHT 400
# define MINI_MAP_BMP_WIDTH 400
# define SURFACE_H_IS_ODD ((BMP_HEIGHT % 2 == 0) ? 0 : 1)
# define TEXTURE_NUM 5
# define ROTATION_ANGLE 0.0372665
# define PLAYER_SPEED 0.1
# define SECOND 1000000000
// # define COS_ROTATION_ANGLE 0.99619469483
// # define COS_N_ROTATION_ANGLE 0.99619469483
// # define SIN_ROTATION_ANGLE 0.08715578

typedef enum	e_cardinal_direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	SKYBOX,
	GROUND
}				t_e_cardinal_direction;

typedef enum	e_intersected_obj_type
{
	WALL,
	PROJECTILE,
	ENEMY,
}				t_e_intersected_obj_type;

typedef struct	s_sdl_data
{
	SDL_Window	*win;
	SDL_Surface	*bmp;
	SDL_Surface	*mini_map_bmp;
	SDL_Surface	*display;
	SDL_Event	event;
	SDL_Surface *menu[2];
	int			quit;
	int			startgame;
}				t_sdl_data;

typedef struct s_action
{
	int			type;
}				t_action;

typedef	struct		s_inter
{
	t_vec3			point;
	double			distance;
	int				intersected;
	int				facing;
}					t_inter;

typedef	struct		s_wall
{
	int				facing;
	double			distance_from_origin;
	t_vec3			inter_point;
	double			wall_height;
}					t_wall;

typedef SDL_Surface t_texture;

typedef struct		s_ray_hit
{
	int				facing;
	int				type;
	double			distance_from_origin;
	t_vec3			point;
	double			wall_height;
}					t_ray_hit;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			dir;
	double			t;
	double			angle;
	t_vec3			first_ver_point;
	t_vec3			first_hor_point;
	t_vec3			increments_v;
	t_vec3			increments_h;
	t_ray_hit		ray_hit;
}					t_ray;

typedef t_vec2int t_index;

typedef struct s_tile
{
	t_vec3int	pos;
	double		height;
	double		width;
	double		depth;
	t_texture	ground;
	t_texture	wall;
	t_index		index;
}				t_tile;

typedef struct s_map
{
	int			height;
	int			width;
	int			skybox;
	t_tile		**tiles;
}				t_map;

typedef enum	e_player_controller_buttons
{
	PLAYER_FORWARD,
	PLAYER_BACKWARDS,
	PLAYER_TURN_LEFT,
	PLAYER_TURN_RIGHT,
	PLAYER_STRAFE_RIGHT,
	PLAYER_STRAFE_LEFT,
	PLAYER_TURN_BACK,
	PLAYER_LOOK_UP,
	PLAYER_LOOK_DOWN,
}				t_e_player_controller_buttons;

typedef struct s_player
{
	t_vec3		pos;
	t_vec3		velocity;
	t_vec3		forw;
    t_vec3      right;
	double		focal_len;
	double		cam_hor;
	double		cam_ver;
	Uint16		controller[11];
	void		(*move)(struct s_player *player);
	void		(*update_velocity)(struct s_player *player, double speed);
	void		(*rotate)(struct s_player *player, double rotation_angle);
	void		(*collision)(struct s_player *player, t_map *map);
	double		rotation_angle;
	double		speed;
	double		height;
}				t_player;

typedef struct	s_thread_data
{
	t_player	*player;
	t_map		*map;
	t_sdl_data	*sdl_data;
	t_vec2int	stend;
}				t_thread_data;

typedef struct	s_text_layer
{
	SDL_Rect	text_pos;
	SDL_Surface *options[3];
	TTF_Font	*font;
	SDL_Color	color[2];
}				t_text_layer;

typedef struct	s_menu_env
{
	t_text_layer	tl;
	Mix_Music		*backgroundsound;
}				t_menu_env;

void			ft_free_surface(t_sdl_data *sdl_data);
void			ft_free_textures(void);
void			ft_update_screen(t_sdl_data *sdl_data);
void 			ft_clear_screen(t_sdl_data *sdl_data);
void 			ft_apply_render(t_sdl_data *sdl_data, t_map *map, t_player *player);
void			ft_apply_physics(t_player *player, t_map *map);
t_index 		get_map_index(t_vec3 position, t_map *map);
t_vec3			ft_limit_inter_by_map(t_vec3 position, t_map *map);
t_index			ft_limit_index_by_map(t_index index, t_map *map);
double			ft_limit_inter(t_vec3 position, double to_add);
void 			ft_draw_walls(int x, t_ray_hit wall, SDL_Surface *bmp, t_player *player);
t_vec3			ft_limit_inter_by_map(t_vec3 position, t_map *map);
t_vec3			ft_map_pixels_to_world(int x, t_player *player);
SDL_Surface 	**get_all_textures(void);
int 			get_sky_texture(int x, int y, SDL_Surface *bmp);
int 			get_wall_texture(int y, t_ray_hit wall, SDL_Surface *bmp);
void			ft_init_bgmusic(Mix_Music *backgroundsound, t_sdl_data sdl_data);
void			ft_init_text_layer(t_text_layer *tl, t_sdl_data sdl_data);
void			ft_menu_loop_content(t_sdl_data *sdl_data, int flags[2], t_menu_env	*menu_env, t_map *map);
void			ft_free_surface(t_sdl_data *sdl_data);
void			ft_graceful_shutdown(t_sdl_data *sdl_data, t_map *map, Mix_Music *backgroundsound);
void			*ft_save_ppm_pixels(void *surface);
void			ft_draw_player(SDL_Surface *surface, int ox, int oy);
void			ft_init_player_controller(t_player *player);
void			ft_player_input(t_player *player, SDL_Event event, SDL_Surface *surface);
void			ft_player_physics(t_player *player, t_map *map);
void			ft_create_player(t_player *player, double x, double y, t_vec3 look_dir);
void			ft_player_move(t_player *player);
void			ft_player_velocity(t_player *player, double speed);
void			ft_player_rotate(t_player *player, double rotation_angle);
void			ft_check_player_collision(t_player *player, t_map *map);
void			putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32			getpixel(SDL_Surface *surface, int x, int y);
Uint32			get_pixel32(SDL_Surface *surface, int x, int y);
void			put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel);
void			ft_sdl_init_data(t_sdl_data *sdl_data);
SDL_Surface		*ft_create_surface(int win_width, int win_height, int bpp);
int				key_code(SDL_Event	event);
int				scan_code(SDL_Event	event);
int				scan_code_up(SDL_Event	event);
int				scan_code_down(SDL_Event	event);
int				ft_shift_is_down(void);
void			ft_print_map(t_map *map, t_player *player);
void			ft_draw_mini_map(SDL_Surface *surface, t_map *map, t_player *player);
void			ft_draw_mini_map_wall_inter(t_ray_hit wall, SDL_Surface *bmp);
t_map			*ft_create_map(char *file, t_player *player);
void			ft_destroy_map(t_map *map);
void			ft_handle_threads(t_player *player, t_map *map, t_sdl_data *sdl_data);
int				ft_ray_cast(t_ray *ray, t_vec3 origin, t_vec3 direction, t_map *map);
void			ft_find_closest_wall(t_ray *ray, t_map *map, t_vec3 forward);
t_map			*ft_get_map_from_file(int fd, t_player *player);
void			ft_put_error(char *traceback);

#endif