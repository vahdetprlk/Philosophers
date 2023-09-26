/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/09/25 19:45:03 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	ft_init_vars(t_vars *vars, char **argv)
{
	vars->n_of_philo = ft_atoi(argv[0]);
	vars->t_to_die = ft_atoi(argv[1]);
	vars->t_to_eat = ft_atoi(argv[2]);
	vars->t_to_sleep = ft_atoi(argv[3]);
	vars->eat_per_phil = ft_atoi(argv[4]);
}

int	main(int argc, char *argv[])
{
	int		i;
	t_vars	*vars;
 
	i = 1;
	if (argc == 5 || argc == 6)
	{
		vars = &(t_vars){0};
		ft_init_vars(vars, &argv[1]);
		while (i < argc)
		{
			printf("%d", ft_atoi(argv[i]));
			i++;
		}

	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
