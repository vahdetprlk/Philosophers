/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:40:01 by vparlak           #+#    #+#             */
/*   Updated: 2023/10/07 14:36:47 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h> // silinecek

typedef struct timeval	t_time;

typedef struct s_die
{
	int	i;
	int	i_m;
}	t_die;

typedef struct s_mutex
{
	pthread_mutex_t	mutex_id;
	int				i_m;
}	t_mutex;

typedef struct s_vars
{
	int		n_of_philo;
	int		t_to_die;
	int		t_to_eat;
	int		t_to_sleep;
	int		eat_per_phil;
}	t_vars;

typedef struct s_philo
{
	pthread_t	thread;
	t_mutex		mutex;
	t_vars		vars;
	t_time		time_start;
	t_time		time_end;
	t_time		starving_start;
	t_time		starving_end;
	int			current_time;
	int			is_eating;
	int			is_died;
}	t_philo;

void	*ft_calloc(size_t count, size_t size);
int		ft_atoi(const char *str);

#endif
