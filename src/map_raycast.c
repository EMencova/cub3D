/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_raycast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:14:46 by yfontene          #+#    #+#             */
/*   Updated: 2024/11/02 21:13:00 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void initialize_ray(t_game *game, int x, t_ray *ray)
{
    calculate_ray_direction(game, x, ray);
   ray->grid_x = (int)game->player.pos_x;
    ray->grid_y = (int)game->player.pos_y;
    
    initialize_dda(game, ray);
}

void calculate_draw_positions(t_ray *ray)
{
    int lineHeight;

    lineHeight = (int)(HEIGHT / ray->wall_dist);
    ray->start_draw = (-lineHeight / 2 + HEIGHT / 2);
    ray->end_draw = (lineHeight / 2 + HEIGHT / 2);
    if (ray->start_draw < 0)
        ray->start_draw = 0;
    if (ray->end_draw >= HEIGHT)
        ray->end_draw = HEIGHT - 1;
}

int perform_raycasting(t_game *game, int x, t_ray *ray)
{
    initialize_ray(game, x, ray);

    return (perform_dda(game, ray));
}

double calculate_wall_x(t_game *game, t_ray *ray)
{
    
    if (ray->wall_side == 1 || ray->wall_side == 2)
        ray->wallx = game->map.player_y + ray->wall_dist * ray->direc_y;
    else
        ray->wallx = game->map.player_x + ray->wall_dist * ray->direc_x;
    ray->wallx -= floor(ray->wallx);
    return (ray->wallx);
}

void draw_scene(t_game *game, int x, t_ray *ray)
{
    ray->wallx = calculate_wall_x(game, ray);
    if (ray->wall_side == 1)
        draw_vertical_line(game, x, ray, game->texture.north_img);
    else if (ray->wall_side == 2)
        draw_vertical_line(game, x, ray, game->texture.south_img);
    else if (ray->wall_side == 3)
        draw_vertical_line(game, x, ray, game->texture.east_img);
    else if (ray->wall_side == 4)
        draw_vertical_line(game, x, ray, game->texture.west_img);
    draw_floor_and_ceiling(game, x, ray);
}

void raycasting(t_game *game)
{
    t_ray ray;
    int x;
    
    x = 0;
    while (x < WIDTH)
    {
        ray.wall_side = perform_raycasting(game, x, &ray); 
        ray.wall_dist = calculate_perpendicular_distance(game, &ray);
        calculate_draw_positions(&ray);
        draw_scene(game, x, &ray);    
        x++;
    }
}
