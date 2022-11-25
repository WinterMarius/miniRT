/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawinter <mawinter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 10:45:02 by mawinter          #+#    #+#             */
/*   Updated: 2022/11/25 22:30:25 by mawinter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

double	get_sphere_intersect( t_sphere *sphere, t_ray ray)
{
	t_vec3 oc = vec3_sub(ray.origin, sphere->position);
	double A = 1.0f;
	double B = 2.0 * vec3_dot(oc, ray.direction);
	double C = vec3_dot(oc, oc) - (sphere->radius) * (sphere->radius);
	double discriminant = B * B - 4 * A * C;
	if (discriminant < 0)
		return (-1.0);
	// printf("sphere: t: %f\n", (-B - sqrt(discriminant)) / (2.0 * A));
	if ((-B - sqrt(discriminant)) / (2.0 * A) < 0.0L)
		return (-B + sqrt(discriminant)) / (2.0 * A);
	return (-B - sqrt(discriminant)) / (2.0 * A);
}

double	get_cylinder_intersect(t_cylinder *cylinder, t_ray ray)
{
	t_ray l_ray;

	l_ray.direction = vec3_matrix_mult(cylinder->m_to_cylinder, ray.direction, 1);
	l_ray.origin = vec3_matrix_mult(cylinder->m_to_cylinder, ray.origin, 0);
	// print_ray(l_ray);
	double a = pow(l_ray.direction.x, 2) + pow(l_ray.direction.y, 2);
	double b = 2.0L * l_ray.origin.x * l_ray.direction.x +  2 * l_ray.origin.y * l_ray.direction.y;
	double c = pow(l_ray.origin.x, 2) + pow(l_ray.origin.y, 2) - pow(cylinder->diameter / 2.0L, 2);
	double discriminant = pow(b, 2) - (4 * a * c);
	// printf("%f\n", sqrt(discriminant)/ (2*a));
	// printf("a%f\n", a);
	if (discriminant < 0)
		return (-1.0);
	// printf("cyliner: t: %f\n", (-b - sqrt(discriminant)) / (2.0 * a));
	// return (-5);
	// printf("a  = %f\n", a);
	// printf("%f\n", (-b - sqrt(discriminant)) / (2.0 * a));
	return (-b - sqrt(discriminant)) / (2.0 * a);
}

double	get_plane_intersect( t_plane *plane, t_ray ray)
{
	//are the value already normalized?
	double	denom;
	double	nom;
	t_vec3	tmp;
	
	denom = vec3_dot(plane->normal_vec, ray.direction);
	if (denom != 0)
	{
		tmp = vec3_sub(plane->position, ray.origin);
		nom = vec3_dot(tmp, plane->normal_vec);
		// printf("plane: t: %f\n", nom / denom);
		return (nom / denom);
	}
	else
		return (-1);
}


t_object *obj_get_nearest(t_object *list, t_ray ray, double *t)
{
	double	tmp;
	int		idx;
	int		minidx;
	t_object	*head;

	head = list;
	idx = 0;
	minidx = 0;
	tmp = -1.0L;
	// printf("START\n");
	while (list)
	{
		// printf("type %c\n", list->type);
		if (list->type == 's')
		{
			tmp = get_sphere_intersect(list->sphere, ray);
			if (tmp >= 0.0L && tmp < *t)
			{
				*t = tmp;
				minidx = idx;
			}
		}
		else if (list->type == 'p')
		{
			tmp = get_plane_intersect(list->plane, ray);
			if (tmp >= 0.0L && tmp < *t)
			{
				*t = tmp;
				minidx = idx;
			}
		}
		else if (list->type =='c')
		{
			tmp = get_cylinder_intersect(list->cylinder, ray);
			if (tmp >= 0.0L && tmp < *t)
			{
				*t = tmp;
				minidx = idx;
			}
		}
		idx++;
		list = list->next;
	}
	return (ft_objat(head, minidx));
}

