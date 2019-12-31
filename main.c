#include "wolf3d.h"

t_vec3	ft_map_pixels_to_world(int x, t_player *player)
{
	double	x_world;
	t_vec3	from_player_to_frame;
	t_vec3	from_frame_center_to_right;
	t_vec3	mapped_pos;

	x_world = player->cam_hor / BMP_WIDTH * (x - BMP_WIDTH / 2);
	from_player_to_frame = ft_vec3_scalar(player->forw, player->focal_len);
	from_frame_center_to_right = ft_vec3_scalar(player->right, x_world);///

	mapped_pos = ft_vec3_add(player->pos,
		ft_vec3_add(from_player_to_frame, from_frame_center_to_right));
	return (mapped_pos);
}

void ft_draw_walls(int x, t_ray_hit wall, SDL_Surface *bmp, t_player *player)
{
	int i;
	int j;
	int counter;
	if (wall.facing == SKYBOX)
		return ;// draw skybox
	i = bmp->h / 2 + player->height;
	j = i + 1;
	counter = 0;
	wall.wall_height = (int)(bmp->h / wall.distance_from_origin);
	while (counter < wall.wall_height / 2)
	{
		if (i >= 0)
		{
			put_pixel32(bmp, x, i, get_wall_texture(i - player->height, wall, bmp));
			i--;
		}
		if (j < bmp->h)
		{
			put_pixel32(bmp, x, j, get_wall_texture(j - player->height, wall, bmp));
			j++;
		}
		counter++;
	}
}

static void	ft_game_loop_content(t_sdl_data *sdl_data, t_player	*player, t_map *map)
{
	while (!sdl_data->quit)
	{
		while (SDL_PollEvent(&sdl_data->event))
		{
			if (sdl_data->event.type == SDL_QUIT || sdl_data->event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				sdl_data->quit = true;
			ft_player_input(player, sdl_data->event, sdl_data->bmp);
		}
		ft_apply_render(sdl_data, map, player);
		ft_apply_physics(player, map);
	}
}

int	main(void)
{
	t_map		*map;
	t_player	player;
	t_sdl_data  sdl_data;
	int			flags[2];
	t_menu_env	menu_env;

	ft_sdl_init_data(&sdl_data);
	menu_env.backgroundsound = NULL;
	flags[0] = 0;
	flags[1] = 0;
	ft_init_text_layer(&menu_env.tl, sdl_data);
	ft_init_bgmusic(menu_env.backgroundsound, sdl_data);
	ft_create_player(&player, TILE_WIDTH + 0.5, TILE_HEIGHT + 0.5, (t_vec3){ -1, 0, 0 });
	if (!(map = ft_create_map("level1.map", &player)))
		return (1);
	if (!get_all_textures())
	{
		sdl_data.quit = true;
		sdl_data.startgame = true;
	}
	ft_menu_loop_content(&sdl_data, flags, &menu_env, map);
	ft_game_loop_content(&sdl_data, &player, map);
	TTF_CloseFont(menu_env.tl.font);
	ft_graceful_shutdown(&sdl_data, map, menu_env.backgroundsound);
}
