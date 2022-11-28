/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawinter <mawinter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 12:51:48 by mawinter          #+#    #+#             */
/*   Updated: 2022/11/28 22:07:23 by mawinter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

void	put_color_pixel(t_data *data, int x, int y, t_color color)
{
	mlx_put_pixel(data->g_img, x, y, color.r << 24 | color.g << 16 | color.b << 8 | 255);
}

// t_color	calc_pixel_color(t_scene *scene)
// {
// 	double	t1;
// 	double	t2;
// 	double	t3;

// 	t1 = 0.0L;
// 	t2 = 0.0L;
// 	t3 = 0.0L;
// 	t1 = get_nearest_sphere();
// 	t2 = get_nearest_plane();
// 	t3 = get_nearest_cylinder();
// 	if (t1 != -1.0L && t1 <= t2 && t2 <= t3)
// 		;
// 	else if (t2 != -1.0L && t2 <= t3 && t3 <= t1)
// 		;
// 	else if (t3 != -1.0L && t3 <= t2 && t2 <= t1)
// 		;
// 	else
// 		printf("I Should Never Be Here\n");
// }

// void	render(t_data *data)
// {
// 	int		x;
// 	int		y;
// 	// t_ray	ray; 
// 	// t_color	pixel_color;

// 	x = 0;
// 	y = 0;
// 	while (y < HEIGHT)
// 	{
// 		while (x < WIDTH)
// 		{
// 			// ray.direction = vec3_matrix_mult(data->scene->camera.m_camera_world,
// 			// 		data->scene->camera.rays[y][x], 1);
// 			// ray.origin = data->scene->camera.v_position;
// 			// pixel_color = calc_pixel_color(data->scene);
// 			// put_color_pixel(data, x, y, pixel_color);
		
// 		}
// 	}
// }
/*
	
*/
t_color	color_of_object(t_object *obj)
{
	if (obj->type == 's')
		return (obj->sphere->color);
	if (obj->type == 'p')
		return (obj->plane->color);
	if (obj->type == 'c')
		return (obj->cylinder->color);
	printf("No object Found");
	return ((t_color) {0,0,0});
}

void	hook(void *param)
{
	t_data	*data;
	data= NULL;
	if(data)
		data = NULL;
	int 	y;
	int 	x;
	t_color	color;
	t_ray	light_ray;

	color.r = 0; color.g = 0; color.b = 255;
	data = param;
	y = 0;
	x = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			t_ray ray;
			ray.origin = data->scene->camera.v_position;
			ray.direction = vec3_matrix_mult(data->scene->camera.m_camera_world,\
												data->scene->camera.rays[y][x], 1);
			//printf("x%lf y%lf z%lf", ray.direction.x, ray.direction.y, ray.direction.z);

			double t = INFINITY;
			t_object *obj =  obj_get_nearest(data->scene->objects, ray, &t);
			if (!obj)
				put_color_pixel(data, x, y, data->scene->ambient_l.ratiocolor);	
			else if (obj)
			{
			// t_hitinfo;
				// printf("hit somehting %f\n", t);
				//calc point
				// vector from potint to light
				light_ray = get_light_ray(t, &ray, data->scene, obj);
				double tnew = INFINITY;
				t_object *newobj =  obj_get_nearest(data->scene->objects, light_ray, &tnew);
				if(!newobj)
				{
					// t_color newcolor;
					// newcolor.r = color_of_object(obj)color_of_object(obj)
					put_color_pixel(data, x, y, color_of_object(obj));
				}
				else
				{
					// double dist;
					// t_vec3 hitpoint = vec3_add(ray.origin, vec3_mult(tnew, ray.direction));
					// t_vec3 surfacenormal = get_surface_normal(newobj, hitpoint);
					// hitpoint = move_by_vec_ratio(hitpoint, EPSILON, surfacenormal);
					// t_vec3	distancevector = vec3_sub(get_obj_position(newobj), hitpoint);
					// dist = vec3_length(distancevector);
					// if (fabs(tnew) >= EPSILON)		
						put_color_pixel(data, x, y, (t_color) {0,0,0});
					// else
						// put_color_pixel(data, x, y,color_of_object(obj));
				}
			}
			x++;
		}
			// printf("");
		y++;
	}
	//printf("%f\n", 1 / data->mlx->delta_time);
}


void	free_data()
{
	return;
}


int	main(int argc, char **argv)
{
	t_data *data;

	if (argc != 2)
		return (1);
	if (argc == 2 && !valid_input(argv[1]))
		return (1);
	else
	data = malloc(sizeof(t_data));
	data->scene = get_scene(argv[1]);
	if (!data->scene)
	{
		printf("GET SCENE UNSUCCESSFULL\n");
		free(data);
		return 1;
	}
	print_scene(data->scene);
	printf("GET SCENE SUCCESSFULL\n");
	t_ray ray;
	ray.origin.x = 0;
	ray.origin.y = 0;
	ray.origin.z = 0;
	ray.direction = (t_vec3) {0, 1, 1};
	//double t = get_cylinder_intersect(ft_objat(data->scene->objects, 2)->cylinder, ray);
	//exit (0);
	//printf("t %f\n", t);
	//print_vec3(vec3_mult(t, ray.direction));
	// return 1;
	data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", false);
	if (!data->mlx)
		exit(EXIT_FAILURE);
	data->g_img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	memset(data->g_img->pixels, 255, data->g_img->width * data->g_img->height * sizeof(int));
	mlx_image_to_window(data->mlx, data->g_img, 0, 0);
	mlx_loop_hook(data->mlx, &hook, data);
	//hook(data);
	mlx_loop(data->mlx);
	mlx_terminate(data->mlx);
	return (EXIT_SUCCESS);
}