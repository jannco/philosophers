/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:00:39 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/02 17:41:21 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	current_timestamp(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL) == -1)
		error_mesage("Failed to get current time");
	return ((current_time.tv_sec * MILLISEC)
		+ (current_time.tv_usec / MILLISEC));
}

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->print_lock);
	printf("%zu %d %s\n", current_timestamp() - data->start_time, id, status);
	pthread_mutex_unlock(&data->print_lock);
}

void	sleep_and_think(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "is sleeping");
	usleep(data->time_to_sleep * MILLISEC);
	print_status(data, philo->id, "is thinking");
}

void	eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->meal_check_lock);
	philo->last_meat_time = current_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->meal_check_lock);
	print_status(data, philo->id, "is eating");
	usleep(data->time_to_eat * MILLISEC);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

void	take_forks(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	print_status(data, philo->id, "has taken a left fork");
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	print_status(data, philo->id, "has taken a right fork");
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (true)
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_create(&data->philosophers[i].threads, NULL,
			philosopher_routine, &data->philosophers[i]);
		i++;
	}
	i = 0;
	while (i < data->number_of_philosophers)
		pthread_join(data->philosophers[i++].threads, NULL);
}

void	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_init(&data->forks[i++], NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_check_lock, NULL);
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = i + 1 % data->number_of_philosophers;
		data->philosophers[i].last_meat_time = data->start_time;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].data = data;
		i++;
	}
}

void	arguments_valid(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) < 1)
		{
			if (i == PHILOSOPHERS)
				error_mesage("Incorrect number of philosophers");
			else if (i == MEALS)
				error_mesage("Invalid number for times to eat");
			else
				error_mesage("Incorrect time");
		}
		i++;
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		error_mesage("Incorrect number of arguments");
	arguments_valid(argv);
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = -1;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->number_of_philosophers); // LEAK
	if (!data->forks)
		error_mesage("Data initialization error");
	data->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->number_of_philosophers); // LEAK
	if (!data->philosophers)
		error_mesage("Data initialization error");
	data->start_time = current_timestamp();
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data, argc, argv);
	init_philosophers(&data);
	init_mutexes(&data);
	start_simulation(&data);
	// cleanup(data);
	return (0);
}
