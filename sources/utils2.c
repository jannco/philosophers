/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:25:38 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/20 15:27:00 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	status_philo(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (status_simulation(data))
		return (true);
	pthread_mutex_lock(&philo->meal_time_lock);
	if (data->number_of_times_must_eat > 0 && philo->meals_eaten
		> data->number_of_times_must_eat)
	{
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_time_lock);
	return (false);
}

bool	status_simulation(t_data *data)
{
	pthread_mutex_lock(&data->simulation_lock);
	if (!data->simulation_run)
	{
		pthread_mutex_unlock(&data->simulation_lock);
		return (true);
	}
	pthread_mutex_unlock(&data->simulation_lock);
	return (false);
}

bool	time_passed(size_t time_to_die, t_philosopher *philo)
{
	size_t	time_passed;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_passed = current_timestamp() - philo->last_meat_time;
	if (time_passed > time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_time_lock);
	return (false);
}
