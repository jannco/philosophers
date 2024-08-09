/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:04:38 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/09 17:19:37 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>

# define PHILOSOPHERS 1
// # define DIE 2
// # define EAT 3
// # define SLEEP 4
# define MEALS 5
// # define THINK 6

# define MILLISEC 1000
// # define NC "\e[0m"
// # define RED "\e[1;31m"

typedef struct s_philosopher
{
	int				id;
	int				right_fork;
	int				left_fork;
	size_t			last_meat_time;
	int				meals_eaten;
	pthread_t		threads;
	pthread_mutex_t	meal_time_lock;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
	int				number_of_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				number_of_times_must_eat;
	bool			simulation_run;
	size_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	t_philosopher	*philosophers;
}	t_data;

int		ft_atoi(const char *str);
int		error_message(char *str);
void	cleanup(t_data *data);
size_t	current_timestamp(void);
void	print_status(t_data *data, int id, char *status);
void	sleep_and_think(t_philosopher *philo);
void	eat(t_philosopher *philo);
void	take_forks(t_philosopher *philo);
void	*philosopher_routine(void *arg);
void	check_status(t_philosopher *philo);
void	*monitoir_routine(void *arg);
void	start_simulation(t_data *data);
void	init_mutexes(t_data *data);
void	init_philosophers(t_data *data);
int		arguments_valid(char **argv);
int		init_data(t_data *data, int argc, char **argv);

#endif
