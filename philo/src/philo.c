/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/05 17:44:16 by vparlak          ###   ########.fr       */
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
		if (pthread_mutex_init(&philo[i].mutex.mutex_id, NULL)) // mutex unlock init olmadıysa 22 döner.
			return (1);
		philo[i].mutex.i_m = (i + 1);
		philo[i].vars = vars_init;
		philo[i].is_eating = 0;
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

int	ft_starve_time(t_philo *philo)
{
	int	starve_time;

	gettimeofday(&philo->starving_end, NULL);
	starve_time = (philo->starving_end.tv_sec * 1000 + philo->starving_end.tv_usec / 1000) -
		(philo->starving_start.tv_sec * 1000 + philo->starving_start.tv_usec / 1000);
	return (starve_time);
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
	philo->is_eating = 1;
	usleep(philo->vars.t_to_eat);
	philo->is_eating = 0;
	gettimeofday(&philo->starving_start, NULL);
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
	}
	return (NULL);
}

/* t_die */int	ft_philo_cycle(t_philo *philo)
{
/* 	t_die	died; */
	int		i;
/* 	int		j; */

/* 	died.i_m = 0;
	died.i = 0; */
	i = 0;
	gettimeofday(&philo[0].time_start, NULL);
	while (i < philo->vars.n_of_philo)
	{
			usleep(10);
			philo[i].time_start = philo[0].time_start;
			philo[i].starving_start = philo[0].time_start;
		if (i % 2 == 0)
			pthread_create(&philo[i].thread, NULL, (void *)ft_life_cycle, &philo[i]); // thread kontrol edilecek initlendiğini
		i++;
	}
	i = 0;
	usleep(50);
	while (i < philo->vars.n_of_philo)
	{	usleep(10);
		if (i % 2 == 1)
			pthread_create(&philo[i].thread, NULL, (void *)ft_life_cycle, &philo[i]); // thread kontrol edilecek initlendiğini
		i++;
	}
	i = 0;
	while (i < philo->vars.n_of_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
		//Not: ölüm için ayrı bir while fonksiyon yan çalışacak.

		/* j = 0;
		while (1)
		{
			if (j == philo->vars.n_of_philo)
				j = 0;
			if ((ft_starve_time(&philo[j]) > (philo[j].vars.t_to_die / 1000)) && philo[j].is_eating == 0)
			{
				died.i_m = j + 1;
				died.i = -1;
				return (died);
			}
			j++;
		} */
	}
	return (i);
}

int	main(int argc, char *argv[])
{
	t_philo	*philo;
	t_vars	vars;
/* 	t_die	died; */
/* 	int		current_time; */

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
		//Not: ölüm için ayrı bir while fonksiyon yan çalışacak. Ölümü kontol edeceğiz.
		/* died =  */ft_philo_cycle(philo);
/* 		if (died.i == -1)
		{
			current_time = (philo[died.i_m].starving_end.tv_sec * 1000 + philo[died.i_m].starving_end.tv_usec / 1000 -
				(philo[died.i_m].time_start.tv_sec * 1000 + philo[died.i_m].time_start.tv_usec / 1000));
			printf("%d %d died\n", current_time, died.i_m);
		} */
		free(philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
