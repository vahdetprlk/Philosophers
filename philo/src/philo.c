/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/02 14:45:59 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

int	ft_init_vars(t_vars *vars, char **argv)
{
	vars->n_of_philo = ft_atoi(argv[0]);
	if (vars->n_of_philo < 1)
		return (write(2, "Philosophers number should 1!\n", 42), 1);
	vars->t_to_die = ft_atoi(argv[1]);
	vars->t_to_eat = ft_atoi(argv[2]);
	vars->t_to_sleep = ft_atoi(argv[3]);
	vars->eat_per_phil = ft_atoi(argv[4]);
	vars->philo = (t_philo *)ft_calloc(vars->n_of_philo, sizeof(t_philo));
}

int	ft_mutex_init(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->n_of_philo)
	{
		if (pthread_mutex_init(&vars->philo[i].mutex.mutex_id , NULL))
			return (1);
		vars->philo[i].mutex.i_m = i;
		i++;
	}
	return (0);
}

void	*ft_life_cycle(t_vars *vars)
{

}

void	ft_philo_cycle(t_vars *vars)
{
	pthread_create(&vars );
}

int	main(int argc, char *argv[])
{
	t_vars	*vars;

	if (argc == 5 || argc == 6)
	{
		vars = &(t_vars){0};
		ft_init_vars(vars, &argv[1]);
		if (!vars->philo)
			return (write(2, "Malloc Error!\n", 14), 1);
		if (ft_mutex_init(vars))
		{
			free(vars->philo);
			return (write(2, "Mutex Error!\n", 13), 1);
		}
		ft_philo_cycle(vars);
		free(vars->philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
