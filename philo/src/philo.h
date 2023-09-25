/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vparlak <vparlak@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:40:01 by vparlak           #+#    #+#             */
/*   Updated: 2023/09/25 17:37:58 by vparlak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <stdio.h> // silinecek

typedef struct s_vars
{
	int	n_of_philo;
	int	t_to_die;
	int	t_to_eat;
	int	t_to_sleep;
	int	eat_per_phil;
}	t_vars;

void	*ft_calloc(size_t count, size_t size);
int		ft_atoi(const char *str);

#endif
