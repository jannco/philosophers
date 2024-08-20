/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:00:39 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/20 14:58:09 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			error_message("Failed to initialize fork mutex");
		pthread_mutex_init(&data->philosophers[i++].meal_time_lock, NULL);
	}
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meals_eaten_lock, NULL);
	pthread_mutex_init(&data->simulation_lock, NULL);
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1)
			% data->number_of_philosophers;
		data->philosophers[i].last_meat_time = data->start_time;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].data = data;
		i++;
	}
}

int	arguments_valid(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) <= 0)
		{
			if (i == PHILOSOPHERS)
				return (error_message("Incorrect number of philosophers"));
			else if (i == MEALS)
				return (error_message("Invalid number for times to eat"));
			else
				return (error_message("Incorrect time"));
		}
		i++;
	}
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (error_message("Incorrect number of arguments"));
	if (arguments_valid(argv))
		return (1);
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_must_eat = -1;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->number_of_philosophers);
	if (!data->forks)
		return (error_message("Failed to allocate memory for forks"));
	data->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->number_of_philosophers);
	if (!data->philosophers)
	{
		free(data->forks);
		return (error_message("Failed to allocate memory for philosophers"));
	}
	data->start_time = current_timestamp();
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (init_data(&data, argc, argv))
		return (1);
	init_philosophers(&data);
	init_mutexes(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
