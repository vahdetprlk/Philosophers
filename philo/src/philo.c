/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/05 11:52:44 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

int	ft_init_vars(t_vars *vars, char **argv)
{
	vars->n_of_philo = ft_atoi(argv[0]);
	if (vars->n_of_philo < 1)
		return (write(2, "Philosophers number should at least 1.\n", 39), 1);
	vars->t_to_die = 1000 * ft_atoi(argv[1]);
	vars->t_to_eat = 1000 * ft_atoi(argv[2]);
	vars->t_to_sleep = 1000 * ft_atoi(argv[3]);
	if (ft_atoi(argv[4]) == 0)
		vars->eat_per_phil = -1;
	else
		vars->eat_per_phil = ft_atoi(argv[4]);
	return (0);
}

int	ft_mutex_init(t_philo *philo, t_vars vars_init)
{
	int	i;

	i = 0;
	while (i < vars_init.n_of_philo)
	{
		if (pthread_mutex_init(&philo[i].mutex.mutex_id, NULL))
			return (1);
		philo[i].mutex.i_m = (i + 1);
		philo[i].vars = vars_init;
		i++;
	}
	return (0);
}

void	ft_current_time(t_philo *philo)
{
	gettimeofday(&philo->time_end, NULL);
	philo->current_time = (philo->time_end.tv_sec * 1000 + philo->time_end.tv_usec / 1000) -
		(philo->time_start.tv_sec * 1000 + philo->time_start.tv_usec / 1000);
}

void	ft_sleep(t_philo *philo)
{
	ft_current_time(philo);
	usleep(philo->vars.t_to_sleep);
	printf("%d %d is sleeping\n", philo->current_time, philo->mutex.i_m);
}
void	ft_eat(t_philo *philo)
{
		ft_current_time(philo);

	usleep(philo->vars.t_to_eat);
	printf("%d %d is eating\n", philo->current_time, philo->mutex.i_m);
	philo->vars.eat_per_phil--;
	pthread_mutex_unlock(&philo->mutex.mutex_id);
	if (philo->mutex.i_m == 1)
		pthread_mutex_unlock(&(philo + (philo->vars.n_of_philo - 1))->mutex.mutex_id);
	else
		pthread_mutex_unlock(&(philo - 1)->mutex.mutex_id);
}
void	*ft_life_cycle(t_philo *philo)
{
	while (1)
	{
		if (philo->mutex.i_m != 1)
		{
				ft_current_time(philo);

			printf("%d %d is thinking\n", philo->current_time, philo->mutex.i_m);
		}
		pthread_mutex_lock(&philo->mutex.mutex_id);
			ft_current_time(philo);

		printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
		if (philo->mutex.i_m == 1)
		{
			pthread_mutex_lock(&(philo + (philo->vars.n_of_philo - 1))->mutex.mutex_id);
				ft_current_time(philo);

			printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
		}
		else
		{
			pthread_mutex_lock(&(philo - 1)->mutex.mutex_id);
				ft_current_time(philo);

			printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
		}
		ft_eat(philo);
		if (philo->vars.eat_per_phil == 0)
			break ;
		ft_sleep(philo);
		if (philo->mutex.i_m == 1)
		{
				ft_current_time(philo);

			printf("%d %d is thinking\n", philo->current_time, philo->mutex.i_m);
		}
	}
	return (NULL);
}

int	ft_philo_cycle(t_philo *philo)
{
	int	i;

	i = 0;

	gettimeofday(&philo[0].time_start, NULL);
	while (i < philo->vars.n_of_philo)
	{
		usleep(50);
		philo[i].time_start = philo[0].time_start;
		pthread_create(&philo[i].thread, NULL, (void *)ft_life_cycle, &philo[i]);
		i++;
	}
	i = 0;
	while (i < philo->vars.n_of_philo)
	{
		while (1)
		{
			// if herhangi bir ölüm olursa bize hbarr et
		}
		pthread_join(philo[i].thread, NULL);

		i++;
	}
	return (i);
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
		if (ft_mutex_init(philo, vars))
		{
			free(philo);
			return (write(2, "Mutex Error!\n", 13), 1);
		}
		ft_philo_cycle(philo);
		// eğer öldüyse dead yaz bitir
		free(philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
