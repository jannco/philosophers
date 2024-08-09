/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yadereve <yadereve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:00:39 by yadereve          #+#    #+#             */
/*   Updated: 2024/08/09 14:13:32 by yadereve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philosophers[i++].meal_time_lock);
	}
	pthread_mutex_destroy(&data->print_lock);
	if (data->forks)
		free(data->forks);
	if (data->philosophers)
		free(data->philosophers);
}

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
		if (data->simulation_run)
			pthread_mutex_unlock(&data->forks[philo->left_fork]);
		data->simulation_run = false;
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
		// usleep(100);
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

void	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philosophers[i++].meal_time_lock, NULL);
	}
	pthread_mutex_init(&data->print_lock, NULL);
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
		data->number_of_times_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_must_eat = -1;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->number_of_philosophers);
	if (!data->forks)
		error_mesage("Data initialization error");
	data->philosophers = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->number_of_philosophers);
	if (!data->philosophers)
		error_mesage("Data initialization error");
	data->start_time = current_timestamp();
	data->simulation_run = true;
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data, argc, argv);
	init_philosophers(&data);
	init_mutexes(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
