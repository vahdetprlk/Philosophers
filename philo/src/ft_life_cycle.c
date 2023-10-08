/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_life_cycle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:21:40 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/08 20:52:50 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	ft_sleep(t_philo *philo)
{
	if (ft_current_time(philo) != 0)
		return (1);
	printf("%d %d is sleeping\n", philo->current_time, philo->mutex.i_m);
	ft_usleep(philo->vars.t_to_sleep);
	return (0);
}

static int	ft_eat(t_philo *philo)
{
	if (ft_died_check(philo) != 0)
		return (1);
	if (ft_current_time(philo) != 0)
		return (1);
	printf("%d %d is eating\n", philo->current_time, philo->mutex.i_m);
	ft_usleep(philo->vars.t_to_eat);
	gettimeofday(&philo->starving_start, NULL);
	if (pthread_mutex_lock(&philo->mutex.eat_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	philo->vars.eat_per_phil--;
	if (pthread_mutex_unlock(&philo->mutex.eat_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	if (pthread_mutex_unlock(&philo->mutex.mutex_id) != 0)
		return (write(2, "Mutex Error\n", 12));
	if (philo->mutex.i_m == 1)
	{
		if (pthread_mutex_unlock(&(philo + (philo->vars.n_of_philo - 1))
				->mutex.mutex_id) != 0)
			return (write(2, "Mutex Error\n", 12));
	}
	else
		if (pthread_mutex_unlock(&(philo - 1)->mutex.mutex_id) != 0)
			return (write(2, "Mutex Error\n", 12));
	return (0);
}

static int	ft_fork_catch(t_philo *philo)
{
	printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
	if (philo->mutex.i_m == 1)
	{
		if (philo->vars.n_of_philo == 1)
			return (1);
		if (pthread_mutex_lock(&(philo + (philo->vars.n_of_philo - 1))
				->mutex.mutex_id) != 0)
			return (write(2, "Mutex Error\n", 12));
		if (ft_current_time(philo) != 0 || ft_died_check(philo) != 0)
			return (1);
		printf("%d %d has taken a fork\n",
			philo->current_time, philo->mutex.i_m);
	}
	else
	{
		if (pthread_mutex_lock(&(philo - 1)->mutex.mutex_id) != 0)
			return (write(2, "Mutex Error\n", 12));
		if (ft_current_time(philo) != 0 || ft_died_check(philo) != 0)
			return (1);
		printf("%d %d has taken a fork\n",
			philo->current_time, philo->mutex.i_m);
	}
	return (0);
}

static int	ft_fork_race(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.mutex_id) != 0)
		return (write(2, "Mutex Error\n", 12));
	if (ft_current_time(philo) != 0)
		return (1);
	if (ft_died_check(philo) != 0)
		return (1);
	if (ft_fork_catch(philo) != 0)
		return (1);
	return (0);
}

void	*ft_life_cycle(t_philo *philo)
{
	while (1)
	{
		if (ft_current_time(philo) != 0 || ft_died_check(philo) != 0)
			return (NULL);
		printf("%d %d is thinking\n", philo->current_time, philo->mutex.i_m);
		if (ft_fork_race(philo) != 0)
			return (NULL);
		if (ft_eat(philo) != 0)
			return (NULL);
		if (ft_last_eat_check(philo) != 0)
			return (NULL);
		if (ft_died_check(philo) != 0)
			return (NULL);
		ft_sleep(philo);
	}
	return (NULL);
}
