/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:56:23 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/20 15:26:57 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(100);
	while (true)
	{
		if (status_simulation(data))
			break ;
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}


bool	check_meals_eaten(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->meals_eaten_lock);
	while (status_philo(&data->philosophers[i]))
	{
		if (i >= (data->number_of_philosophers - 1))
		{
			pthread_mutex_unlock(&data->meals_eaten_lock);
			return (true);
		}
		i++;
	}
	pthread_mutex_unlock(&data->meals_eaten_lock);
	return (false);
}

void	check_status(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->number_of_times_must_eat >= 0)
	{
		if (check_meals_eaten(data))
		{
			pthread_mutex_lock(&data->simulation_lock);
			data->simulation_run = false;
			pthread_mutex_unlock(&data->simulation_lock);
		}
	}
	if (time_passed(data->time_to_die, philo))
	{
		print_status(data, philo->id, "\e[1;31mdied\e[0m");
		pthread_mutex_lock(&data->simulation_lock);
		data->simulation_run = false;
		pthread_mutex_unlock(&data->simulation_lock);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
	}
}

void	*monitoir_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (true)
	{
		i = 0;
		if (status_simulation(data))
			break ;
		while (i < data->number_of_philosophers)
			check_status(&data->philosophers[i++]);
		usleep(100);
	}
	return (NULL);
}

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitoir_threads;

	i = 0;
	data->simulation_run = true;
	if (pthread_create(&monitoir_threads, NULL, monitoir_routine, data))
		return (error_message("Failed to create monitor thread"));
	while (i < data->number_of_philosophers)
	{
		if (pthread_create(&data->philosophers[i].threads, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (error_message("Failed to create philosopher thread"));
		i++;
	}
	i = 0;
	pthread_join(monitoir_threads, NULL);
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].threads, NULL);
		i++;
	}
	return (0);
}
