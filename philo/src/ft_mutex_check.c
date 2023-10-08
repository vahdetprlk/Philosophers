/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mutex_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/08 20:53:28 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	ft_died_check(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	if (philo->is_died == 1)
	{
		if (pthread_mutex_unlock(&philo->mutex.death_mutex) != 0)
			return (write(2, "Mutex Error\n", 12));
		return (1);
	}
	if (pthread_mutex_unlock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	return (0);
}


int	ft_died_eat_check(t_philo *philo, int i)
{
	if (pthread_mutex_lock(&philo->mutex.eat_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	if (philo->vars.eat_per_phil != 0)
	{
		if (pthread_mutex_unlock(&philo->mutex.eat_mutex) != 0)
			return (write(2, "Mutex Error\n", 12));
		printf("%d %d died\n", philo[i].die_time, philo[i].mutex.i_m);
	}
	else
		if (pthread_mutex_unlock(&philo->mutex.eat_mutex) != 0)
			return (write(2, "Mutex Error\n", 12));
	return (0);
}

int	ft_last_eat_check(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.eat_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	if (philo->vars.eat_per_phil == 0)
	{
		if (pthread_mutex_unlock(&philo->mutex.eat_mutex) != 0)
			return (write(2, "Mutex Error\n", 12));
		return (1);
	}
	if (pthread_mutex_unlock(&philo->mutex.eat_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	return (0);
}

int	ft_detach_pthread(t_philo *philo)
{
	int	i;

	i = 0;
	while (philo->vars.n_of_philo)
	{
		pthread_detach(philo[i].thread);
		i++;
	}
	return (0);
}
