/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:04:38 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/02 15:48:03 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>

# define PHILOSOPHERS 1
# define DIE 2
# define EAT 3
# define SLEEP 4
# define MEALS 5

# define MILLISEC 1000

typedef struct s_philosopher
{
	int				id;
	int				right_fork;
	int				left_fork;
	size_t			last_meat_time;
	int				meals_eaten;
	pthread_t		threads;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
	int				number_of_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			number_of_times_each_philosopher_must_eat;
	size_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_check_lock;
	t_philosopher	*philosophers;
}	t_data;

int		ft_atoi(const char *str);
void	error_mesage(char *str);

#endif
