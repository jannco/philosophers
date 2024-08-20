/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:00:29 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/20 14:58:35 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_message(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		write(STDERR_FILENO, &str[i++], 1);
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philosophers[i].meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meals_eaten_lock);
	pthread_mutex_destroy(&data->simulation_lock);
	if (data->forks)
		free(data->forks);
	if (data->philosophers)
		free(data->philosophers);
}

void	ft_usleep(suseconds_t time_to_wait)
{
	suseconds_t	start_time;

	start_time = (suseconds_t)current_timestamp();
	while (((suseconds_t)current_timestamp() - start_time) < time_to_wait)
		usleep(100);
}

size_t	current_timestamp(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL) == -1)
		error_message("Failed to get current time");
	return ((current_time.tv_sec * MILLISEC)
		+ (current_time.tv_usec / MILLISEC));
}

int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	negative;

	negative = 1;
	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] > 8 && str[i] < 14))
		i++;
	if (str[i] == '-')
		negative = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * negative);
}
