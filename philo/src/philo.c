/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/03 13:48:57 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

int	ft_init_vars(t_vars *vars, char **argv)
{
	vars->n_of_philo = ft_atoi(argv[0]);
	if (vars->n_of_philo < 1)
		return (write(2, "Philosophers number should at least 1!\n", 42), 1);
	vars->t_to_die = 1000 * ft_atoi(argv[1]);
	vars->t_to_eat = 1000 * ft_atoi(argv[2]);
	vars->t_to_sleep = 1000 * ft_atoi(argv[3]);
	vars->eat_per_phil = ft_atoi(argv[4]);
	return (0);
}

int	ft_mutex_init(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->vars.n_of_philo)
	{
		if (pthread_mutex_init(&philo[i].mutex.mutex_id, NULL))
			return (1);
		philo[i].mutex.i_m = (i + 1);
		i++;
	}
	return (0);
}

void	*ft_life_cycle(t_philo *philo)
{
	 int flag = 1;

	while (1)
	{
		if (flag == philo->mutex.i_m)
			printf("Philo %d is thinking.\n", philo->mutex.i_m);
		pthread_mutex_lock(&philo->mutex.mutex_id);
		printf("Philo %d has taken a fork.\n", philo->mutex.i_m);
		if (philo->mutex.i_m == 1)
		{
			pthread_mutex_lock(&(philo + (philo->vars.n_of_philo - 1))->mutex.mutex_id);
			printf("Philo %d has taken a fork.\n", philo->mutex.i_m);
		}
		else
		{
			pthread_mutex_lock(&(philo - 1)->mutex.mutex_id);
			printf("Philo %d has taken a fork.\n", philo->mutex.i_m);
		}
		printf("Philo %d is eating.\n", philo->mutex.i_m);
		usleep(philo->vars.t_to_eat);
		pthread_mutex_unlock(&philo->mutex.mutex_id);
		if (philo->mutex.i_m == 1)
			pthread_mutex_unlock(&(philo + (philo->vars.n_of_philo - 1))->mutex.mutex_id);
		else
			pthread_mutex_unlock(&(philo - 1)->mutex.mutex_id);
		printf("Philo %d is sleeping.\n", philo->mutex.i_m);
		usleep(philo->vars.t_to_sleep);
		flag = philo->mutex.i_m;
	}
	return (NULL);
}

void	ft_philo_cycle(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->vars.n_of_philo)
	{
		usleep(50);
		pthread_create(&philo[i].thread, NULL,
			(void *)ft_life_cycle, &philo[i]);
		i++;
	}
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
		philo->vars = vars;
		if (ft_mutex_init(philo))
		{
			free(philo);
			return (write(2, "Mutex Error!\n", 13), 1);
		}
		ft_philo_cycle(philo);
		sleep(3);
		free(philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
