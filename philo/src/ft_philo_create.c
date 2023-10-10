/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:12:19 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/10 19:47:54 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_died_set(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	philo->is_died = 1;
	if (pthread_mutex_unlock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	return (0);
}

static int	ft_starving_check(t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (1)
	{
		if (philo->vars.n_of_philo == i)
			i = 0;
		if (pthread_mutex_lock(&philo[i].mutex.eat_mutex) != 0)
			return (write(2, "Mutex Error\n", 12));
		j = ft_starve_time_check(philo, i);
		if (j > 0)
			return (1);
		else if (j == -1)
			break ;
		if (ft_n_times_eat_check(philo) == 1)
			break ;
		i++;
	}
	return (0);
}

static int	ft_thread_create_single(t_philo *philo)
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

static int	ft_thread_create(t_philo *philo)
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
	if (ft_thread_create_single(philo) != 0)
		return (1);
	return (0);
}

int	ft_philo_create(t_philo *philo)
{
	int	i;

	if (ft_thread_create(philo) != 0)
		return (1);
	if (ft_starving_check(philo) != 0)
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
