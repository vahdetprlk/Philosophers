/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/06 19:24:52 by vparlak          ###   ########.fr       */
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
			return (write(2, "Number of times each philo should at least 0.\n", 46), 1);
	}
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
		if (pthread_mutex_init(&philo[i].mutex.time_mutex, NULL))
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
	pthread_mutex_lock(&philo->mutex.time_mutex); //#################
	gettimeofday(&philo->time_end, NULL);
	philo->current_time = (philo->time_end.tv_sec * 1000 + philo->time_end.tv_usec / 1000) -
		(philo->time_start.tv_sec * 1000 + philo->time_start.tv_usec / 1000);
	pthread_mutex_unlock(&philo->mutex.time_mutex); //#################
}
int	ft_get_local_time(void)
{
	t_time time;
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

void	ft_sleep(t_philo *philo)
{
	ft_current_time(philo);
	printf("%d %d is sleeping\n", philo->current_time, philo->mutex.i_m);
	ft_usleep(philo->vars.t_to_sleep);
}
void	ft_eat(t_philo *philo)
{
	ft_current_time(philo);
	philo->is_eating = 1;
	pthread_mutex_lock(&philo->mutex.time_mutex); //#################
	printf("%d %d is eating\n", philo->current_time, philo->mutex.i_m);
	pthread_mutex_unlock(&philo->mutex.time_mutex); //#################
	ft_usleep(philo->vars.t_to_eat);
	gettimeofday(&philo->starving_start, NULL);
	philo->is_eating = 0;
	philo->vars.eat_per_phil--;
	pthread_mutex_unlock(&philo->mutex.mutex_id);
	if (philo->mutex.i_m == 1)
		pthread_mutex_unlock(&(philo + (philo->vars.n_of_philo - 1))->mutex.mutex_id);
	else
		pthread_mutex_unlock(&(philo - 1)->mutex.mutex_id);
}

int	ft_starve_time(t_philo *philo)
{
	int	starve_time;

	gettimeofday(&philo->starving_end, NULL);
	starve_time = (philo->starving_end.tv_sec * 1000 + philo->starving_end.tv_usec / 1000) -
		(philo->starving_start.tv_sec * 1000 + philo->starving_start.tv_usec / 1000);
	return (starve_time);
}

void	*ft_death_check(t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	while (1)
	{
		if(philo->vars.n_of_philo == i)
			i = 0;
		if(ft_starve_time(&philo[i]) > philo[i].vars.t_to_die)
		{
			ft_current_time(&philo[i]);
			j = 0;
			while (j < philo->vars.n_of_philo)
			{
				pthread_mutex_lock(&philo[j].mutex.time_mutex); //#################
				philo[j].is_died = 1;
				pthread_mutex_unlock(&philo[j].mutex.time_mutex); //#################
				j++;
			}
			pthread_mutex_lock(&philo[i].mutex.time_mutex); //#################
			printf("%d %d died\n", philo[i].current_time, philo[i].mutex.i_m);
			pthread_mutex_unlock(&philo[i].mutex.time_mutex); //#################
			break ;
		}
		i++;
	}
	return (NULL);
}


void	*ft_life_cycle(t_philo *philo)
{
	while (1)
	{
		ft_current_time(philo);
		if (philo->is_died == 1)
			break ;
		pthread_mutex_lock(&philo->mutex.time_mutex); //#################
		printf("%d %d is thinking\n", philo->current_time, philo->mutex.i_m);
		pthread_mutex_unlock(&philo->mutex.time_mutex); //#################
		pthread_mutex_lock(&philo->mutex.mutex_id);
		ft_current_time(philo);
		pthread_mutex_lock(&philo->mutex.time_mutex); //#################
		if (philo->is_died == 1)
			break ;
		pthread_mutex_unlock(&philo->mutex.time_mutex); //#################
		printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
		if (philo->mutex.i_m == 1)
		{
			if(philo->vars.n_of_philo == 1)
			{
				printf("%d %d died\n", philo->vars.t_to_die, philo->mutex.i_m);
				break;
			}
			pthread_mutex_lock(&(philo + (philo->vars.n_of_philo - 1))->mutex.mutex_id);
			ft_current_time(philo);
			if (philo->is_died == 1)
			break ;
			printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
		}
		else
		{
			pthread_mutex_lock(&(philo - 1)->mutex.mutex_id);
			ft_current_time(philo);
			if (philo->is_died == 1)
			break ;
			printf("%d %d has taken a fork\n", philo->current_time, philo->mutex.i_m);
		}
		if (philo->is_died == 1)
			break ;
		ft_eat(philo);
		pthread_mutex_lock(&philo->mutex.time_mutex); //#################
		if (philo->vars.eat_per_phil == 0 || philo->is_died == 1)
			break ;
		pthread_mutex_unlock(&philo->mutex.time_mutex); //#################
		ft_sleep(philo);
	}
	return (NULL);
}

int	ft_philo_cycle(t_philo *philo)
{
	pthread_t 	time_pthread;
	int			i;


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
	{
		usleep(10);
		if (i % 2 == 1)
			pthread_create(&philo[i].thread, NULL, (void *)ft_life_cycle, &philo[i]); // thread kontrol edilecek initlendiğini
		i++;
	}
	pthread_create(&time_pthread, NULL, (void *)ft_death_check, &philo[0]); // thread kontrol edilecek initlendiğini
	pthread_detach(time_pthread);
	i = 0;
	while (i < philo->vars.n_of_philo)
	{
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
		free(philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
