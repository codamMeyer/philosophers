#include <philosopher.h>
#include <philosophers_actions.h>
#include <stdlib.h>
#include <stdio.h>
#include <display.h>
#include <time_utils.h>

static void	setup_philosopher(t_philo *philosopher, \
							int id, \
							t_display *display, \
							t_philo_config *config)
{
	philosopher->id = id;
	philosopher->last_meal.value = 0;
	philosopher->meals_counter = 0;
	philosopher->display = display;
	philosopher->config = config;
}

static void	assign_forks(t_philo *philosopher, \
						t_fork *forks, \
						int i, \
						int last_philo)
{
	philosopher->forks.right = &forks[i];
	if (i == 0)
		philosopher->forks.left = &forks[last_philo];
	else
		philosopher->forks.left = &forks[i - 1];
}

t_philo	*create_philosophers(t_philo_config *config, \
							t_fork *forks, \
							t_display *display)
{
	const int	num_philosophers = config->number_of_philosophers;
	t_philo		*philosophers;
	int			i;

	philosophers = malloc(sizeof(t_philo) * num_philosophers);
	if (!forks || !philosophers)
		return (NULL);
	i = 0;
	while (i < num_philosophers)
	{
		setup_philosopher(&philosophers[i], i + 1, display, config);
		assign_forks(&philosophers[i], forks, i, num_philosophers - 1);
		++i;
	}
	return (philosophers);
}

t_bool	is_dinner_over(t_philo *philo)
{
	if (philo->config->min_meals && philo->config->need_to_finish_meals == 0)
		return (TRUE);
	if (is_dead(philo) || philo->config->death_event)
		return (TRUE);
	return (FALSE);
}

void	*start_dinner(void *philo)
{
	unsigned int	cur_time;
	t_philo			*philosopher;

	philosopher = (t_philo *)philo;
	while (!is_dinner_over(philosopher))
	{
		if (get_forks(philosopher))
		{
			start_to_eat(philosopher);
			if (start_to_sleep(philosopher) == DEAD)
			{
				cur_time = get_cur_time(&(philosopher)->config->time_start);
				display_action_message(cur_time, (philosopher), DIED);
				break ;
			}
			start_to_think(philosopher);
		}
	}
	return (NULL);
}

t_status	create_philosophers_threads(t_philo *philosophers, \
										t_pthread_create create_thread)
{
	const int	num_philosophers = philosophers->config->number_of_philosophers;
	int			ret;
	int			i;

	i = 0;
	while (i < num_philosophers)
	{
		ret = create_thread(&(philosophers[i].thread_id), \
							NULL, &start_dinner, &philosophers[i]);
		if (ret)
		{
			philosophers->config->death_event = TRUE;
			philosophers->config->initialized_threads = i;
			return (ERROR);
		}
		++i;
	}
	philosophers->config->initialized_threads = num_philosophers;
	philosophers->display->is_used = FALSE;
	return (SUCCESS);
}
