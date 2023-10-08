/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:42:05 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/08 15:52:23 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

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
			return (write(2, "N_of_t_each philo should at least 0.\n", 37), 1);
	}
	return (0);
}

int	ft_destroy_mutex(t_philo *philo)
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

int	ft_mutex_init(t_philo *philo, t_vars vars_init)
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

int	ft_get_local_time(void)
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

int	ft_sleep(t_philo *philo)
{
	if (ft_current_time(philo) != 0)
		return (1);
	printf("%d %d is sleeping\n", philo->current_time, philo->mutex.i_m);
	ft_usleep(philo->vars.t_to_sleep);
	return (0);
}

int	ft_eat(t_philo *philo)
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

int	ft_died_set(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex.death_mutex) != 0)
		return (write(2, "Mutex Error\n", 12));
	philo->is_died = 1;
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

int	ft_death_check(t_philo *philo)
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

int	ft_think(t_philo *philo)
{
	if (ft_current_time(philo) != 0)
		return (1);
	if (ft_died_check(philo) != 0)
		return (1);
	printf("%d %d is thinking\n", philo->current_time, philo->mutex.i_m);
	return (0);
}

int	ft_fork_catch(t_philo *philo)
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

int	ft_fork_race(t_philo *philo)
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

void	*ft_life_cycle(t_philo *philo)
{
	while (1)
	{
		if (ft_think(philo) != 0)
			return (NULL);
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

int	ft_create_thread_single(t_philo *philo)
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

int	ft_create_thread(t_philo *philo)
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
		ft_destroy_mutex(philo);
		free(philo);
	}
	else
		write(2, "You must enter 4 or 5 arguments!\n", 33);
}
