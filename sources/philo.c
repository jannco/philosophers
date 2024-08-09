/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:58:01 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/09 17:03:09 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->print_lock);
	if (data->simulation_run)
		printf("%zu\t%d %s\n", current_timestamp() - data->start_time, id,
			status);
	pthread_mutex_unlock(&data->print_lock);
}

void	sleep_and_think(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (!data->simulation_run)
		return ;
	print_status(data, philo->id, "is sleeping");
	usleep(data->time_to_sleep * MILLISEC);
	if (data->simulation_run)
		print_status(data, philo->id, "is thinking");
}

void	eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meat_time = current_timestamp();
	philo->meals_eaten++;
	print_status(data, philo->id, "is eating");
	pthread_mutex_unlock(&philo->meal_time_lock);
	usleep(data->time_to_eat * MILLISEC);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

void	take_forks(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	print_status(data, philo->id, "has taken a fork");
}
