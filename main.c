#include "wolf3d.h"

typedef struct s_action
{
	int			type;
}				t_action;

typedef struct s_player
{
	t_vec3		pos;
	t_vec3		dir;
	double		height;
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

void	ft_destroy_map(t_map *map)
{
	int i;
	int j;

	i = -1;
	while (++i < map->height)
	{
		j = -1;
		while (++j < map->width)
		{
			free(map->tiles[j]);
			map->tiles[j] = NULL;
		}
	}
	free(map->tiles);
	map->tiles = NULL;
	free(map);
	map = NULL;
}

t_map	*ft_create_map(int width, int height)
{
	t_map *map;
	int i;
	int j;

	if (height < 4 || width < 4)
		return (NULL);
	map = (t_map *)malloc(sizeof(t_map));


	map->tiles = (t_tile **)malloc(sizeof(t_tile *) * height);
	map->height = height;
	map->width = width;
	i = -1;
	while (++i < height)
	{
		j = -1;
		map->tiles[i] = (t_tile *)malloc(sizeof(t_tile) * width);
		while (++j < width)
		{
			map->tiles[i][j].width = 100;
			map->tiles[i][j].height = 100;
			map->tiles[i][j].depth = 0;
			map->tiles[i][j].pos = (t_vec3int){ i, j, 0 };
		}
	}
	return (map);
}

void	ft_player_controller(t_player *player, SDL_Event event)
{
	(void)player;
	switch (event.type)
	{
		case SDL_KEYDOWN:
			printf("key down\n");
			break;
		case SDL_KEYUP:
			printf("key up\n");
			break;
	}
}

t_player *ft_create_player()
{
	t_player *player;

	player = (t_player *)malloc(sizeof(t_player));
	player->pos = (t_vec3){ 0, 0, 0 };
	player->dir = (t_vec3){ 1, 0, 0 };
	return (player);
}

int	game_manager(t_action *actions)
{
	// if (player's action is OK)
	// 	return (1);
	// return (0);
}

int	main()
{
	SDL_Window	*win;
	SDL_Surface	*bmp;
	SDL_Event	event;
	SDL_Surface *display;
	t_map		*map;
	t_player	*player;
	int i = -1, j;

	player = ft_create_player();
	win = ft_sdl_init_create_window(WIN_WIDTH, WIN_HEIGHT);
	bmp = ft_create_surface(win, WIDTH, HEIGHT, BPP);
	display = SDL_GetWindowSurface(win);
	map = ft_create_map(30, 30);
	while (1337)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break ;
			ft_player_controller(player, event);
		}
		while (i++ < bmp->h)
		{
			j = -1;
			while (j++ < bmp->w)
				putpixel(bmp, j, i, 0x09FF0000);
		}
		SDL_BlitSurface(bmp, NULL, display, NULL);
		SDL_UpdateWindowSurface(win);
	}


	ft_destroy_map(map);
	SDL_FreeSurface(bmp);
	SDL_DestroyWindow(win);
	SDL_Quit();
}


// ft_gui(bmp, display);
// SDL_BlitSurface(bmp, NULL, display, NULL);
// TTF_CloseFont(font);
// SDL_FreeSurface(ui_surface);




// int l;
// int k;

// l = -1;
// while (++l < map->height)
// {
// 	k = -1;
// 	while (++k < map->width)
// 	{
// 		printf("pos ( %d, %d, %d )\n", map->tiles[l][k].pos.x, map->tiles[l][k].pos.y, map->tiles[l][k].pos.z);
// 	}
// }


// void ft_gui(TTF_Font *font, SDL_Surface *display)
// {

// 	font = TTF_OpenFont("arial.ttf", 25);
// 	SDL_Color color = { 255, 255, 255 };
// 	SDL_Surface * surface = TTF_RenderText_Solid(font, "Welcome to Gigi Labs", color);
// 	// SDL_Texture * texture = SDL_CreateTextureFromSurface(display, surface);
// 	// SDL_DestroyTexture(texture);
// 	return (surface);
// }