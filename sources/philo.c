/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:58:01 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/20 15:26:31 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	print_status(t_data *data, int id, char *status)
{
	if (!status_simulation(data))
	{
		pthread_mutex_lock(&data->print_lock);
		printf("%zu\t%d %s\n", current_timestamp() - data->start_time, id,
			status);
		pthread_mutex_unlock(&data->print_lock);
	}
}

void	sleep_and_think(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (!status_philo(philo))
		print_status(data, philo->id, "is sleeping");
	ft_usleep(data->time_to_sleep);
	if (!status_philo(philo))
		print_status(data, philo->id, "is thinking");
}

void	eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meat_time = current_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (!status_philo(philo))
		print_status(data, philo->id, "is eating");
	ft_usleep(data->time_to_eat);
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
