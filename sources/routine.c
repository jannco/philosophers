/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:56:23 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/09 17:03:05 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (philo->data->simulation_run)
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}

void	check_status(t_philosopher *philo)
{
	t_data	*data;
	size_t	time_passed;

	data = philo->data;
	pthread_mutex_lock(&philo->meal_time_lock);
	time_passed = current_timestamp() - philo->last_meat_time;
	if (data->number_of_times_must_eat > 0)
	{
		if (philo->meals_eaten > data->number_of_times_must_eat)
			data->simulation_run = false;
	}
	if (time_passed > data->time_to_die)
	{
		print_status(data, philo->id, "\e[1;31mdied\e[0m");
		data->simulation_run = false;
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
	}
	pthread_mutex_unlock(&philo->meal_time_lock);
}

void	*monitoir_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (data->simulation_run)
	{
		i = 0;
		while (i < data->number_of_philosophers)
			check_status(&data->philosophers[i++]);
		usleep(100);
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitoir_threads;

	i = 0;
	pthread_create(&monitoir_threads, NULL, monitoir_routine, data);
	while (i < data->number_of_philosophers)
	{
		pthread_create(&data->philosophers[i].threads, NULL,
			philosopher_routine, &data->philosophers[i]);
		i++;
	}
	i = 0;
	pthread_join(monitoir_threads, NULL);
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].threads, NULL);
		i++;
	}
}
