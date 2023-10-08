/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_cycle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:12:19 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/08 20:51:40 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_died_set(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	philo->is_died = 1;
	if (pthread_mutex_unlock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	return (0);
}

static int	ft_death_check(t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	while (1)
	{
		if (philo->vars.n_of_philo == i)
			i = 0;
		if (ft_starve_time(&philo[i]) > philo[i].vars.t_to_die)
		{
			if (ft_die_time(&philo[i]) != 0)
				return (write(2, "Mutex Error\n", 12));
			j = -1;
			while (++j < philo->vars.n_of_philo)
			{
				if (ft_died_set(&philo[j]) != 0)
					return (1);
			}
			if (ft_died_eat_check(philo, i) != 0)
				return (1);
			break ;
		}
		i++;
	}
	return (0);
}

static int	ft_create_thread_single(t_philo *philo)
{
	int	i;

	i = 0;
	usleep(50);
	while (i < philo->vars.n_of_philo)
	{
		usleep(10);
		if (i % 2 == 1)
			if (pthread_create(&philo[i].thread, NULL, (void *)ft_life_cycle,
					&philo[i]))
				return (write(2, "Thread Error!\n", 14));
		i++;
	}
	return (0);
}

static int	ft_create_thread(t_philo *philo)
{
	int	i;

	i = 0;
	gettimeofday(&philo[0].time_start, NULL);
	while (i < philo->vars.n_of_philo)
	{
		usleep(10);
		philo[i].time_start = philo[0].time_start;
		philo[i].starving_start = philo[0].time_start;
		if (i % 2 == 0)
			if (pthread_create(&philo[i].thread, NULL, (void *)ft_life_cycle,
					&philo[i]) != 0)
				return (write(2, "Thread Error!\n", 14));
		i++;
	}
	if (ft_create_thread_single(philo) != 0)
		return (1);
	return (0);
}

int	ft_philo_cycle(t_philo *philo)
{
	int	i;

	if (ft_create_thread(philo) != 0)
		return (1);
	if (ft_death_check(philo) != 0)
		return (1);
	i = 0;
	while (i < philo->vars.n_of_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (write(2, "Join Error!\n", 12));
		i++;
	}
	return (0);
}
