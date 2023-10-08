/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:02:47 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/08 20:51:51 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static int	ft_destroy_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	while (philo->vars.n_of_philo > i)
	{
		pthread_mutex_destroy(&philo[i].mutex.mutex_id);
		pthread_mutex_destroy(&philo[i].mutex.death_mutex);
		pthread_mutex_destroy(&philo[i].mutex.mutex_time);
		pthread_mutex_destroy(&philo[i].mutex.eat_mutex);
		i++;
	}
	return (0);
}

static int	ft_mutex_init(t_philo *philo, t_vars vars_init)
{
	int	i;

	i = 0;
	while (i < vars_init.n_of_philo)
	{
		if (pthread_mutex_init(&philo[i].mutex.mutex_id, NULL) != 0)
			return (1);
		if (pthread_mutex_init(&philo[i].mutex.death_mutex, NULL) != 0)
			return (1);
		if (pthread_mutex_init(&philo[i].mutex.mutex_time, NULL) != 0)
			return (1);
		if (pthread_mutex_init(&philo[i].mutex.eat_mutex, NULL) != 0)
			return (1);
		philo[i].mutex.i_m = (i + 1);
		philo[i].vars = vars_init;
		i++;
	}
	return (0);
}

static int	ft_init(t_philo *philo, t_vars vars)
{
	if (ft_mutex_init(philo, vars))
	{
		ft_destroy_mutex(philo);
		free(philo);
		return (write(2, "Mutex Error!\n", 13), 1);
	}
	if (ft_philo_cycle(philo) != 0)
	{
		ft_destroy_mutex(philo);
		ft_detach_pthread(philo);
		free (philo);
		return (1);
	}
	return (0);
}

static int	ft_init_vars(t_vars *vars, char **argv)
{
	vars->n_of_philo = ft_atoi(argv[0]);
	if (vars->n_of_philo < 1)
		return (write(2, "Philosophers number should at least 1.\n", 39), 1);
	vars->t_to_die = ft_atoi(argv[1]);
	if (vars->t_to_die < 0)
		return (write(2, "Time to die number should at least 0.\n", 38), 1);
	vars->t_to_eat = ft_atoi(argv[2]);
	if (vars->t_to_eat < 0)
		return (write(2, "Time to eat number should at least 0.\n", 38), 1);
	vars->t_to_sleep = ft_atoi(argv[3]);
	if (vars->t_to_sleep < 0)
		return (write(2, "Time to sleep number should at least 0.\n", 40), 1);
	if (ft_atoi(argv[4]) == 0)
		vars->eat_per_phil = -1;
	else
	{
		vars->eat_per_phil = ft_atoi(argv[4]);
		if (vars->eat_per_phil < 0)
			return (write(2, "N_of_t_each philo should at least 0.\n", 37), 1);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo	*philo;
	t_vars	vars;

	if (argc == 5 || argc == 6)
	{
		if (ft_init_vars(&vars, &argv[1]))
			return (1);
		philo = (t_philo *)ft_calloc(vars.n_of_philo, sizeof(t_philo));
		if (!philo)
			return (write(2, "Malloc Error!\n", 14), 1);
		if (ft_init(philo, vars) != 0)
			return (1);
		ft_destroy_mutex(philo);
		free(philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
