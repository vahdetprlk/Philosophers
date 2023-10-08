/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:30:57 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/08 20:51:45 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_get_local_time(void)
{
	t_time	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(int time)
{
	int	start;

	start = ft_get_local_time();
	while (ft_get_local_time() - start < time)
		usleep(time);
}

int	ft_starve_time(t_philo *philo)
{
	int	starve_time;

	gettimeofday(&philo->starving_end, NULL);
	starve_time = (philo->starving_end.tv_sec * 1000
			+ philo->starving_end.tv_usec / 1000)
		- (philo->starving_start.tv_sec * 1000
			+ philo->starving_start.tv_usec / 1000);
	return (starve_time);
}

int	ft_die_time(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.mutex_time) != 0)
		return (write(2, "Mutex Error\n", 12));
	gettimeofday(&philo->time_end, NULL);
	philo->die_time = (philo->time_end.tv_sec * 1000
			+ philo->time_end.tv_usec / 1000) - (philo->time_start.tv_sec * 1000
			+ philo->time_start.tv_usec / 1000);
	if (pthread_mutex_unlock(&philo->mutex.mutex_time) != 0)
		return (write(2, "Mutex Error\n", 12));
	return (0);
}

int	ft_current_time(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.mutex_time) != 0)
		return (write(2, "Mutex Error\n", 12));
	gettimeofday(&philo->time_end, NULL);
	philo->current_time = (philo->time_end.tv_sec * 1000
			+ philo->time_end.tv_usec / 1000) - (philo->time_start.tv_sec * 1000
			+ philo->time_start.tv_usec / 1000);
	if (pthread_mutex_unlock(&philo->mutex.mutex_time) != 0)
		return (write(2, "Mutex Error\n", 12));
	return (0);
}
