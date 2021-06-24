#include <stdio.h>
#include <stdlib.h>
#include <display.h>
#include <parser.h>
#include <fork.h>
#include <types.h>
#include <philosopher.h>
#include <pthread.h>
#include <unistd.h>

static void	cleanup(t_fork *forks, t_philo *philosophers, t_display *display)
{
	const int	num_philosophers = philosophers->config->number_of_philosophers;

	destroy_forks(forks, num_philosophers);
	free(philosophers);
	destroy_display(display);
}

static void	join_philosophers_threads(t_philo *philosophers, int num_threads)
{
	int	i;

	i = 0;
	while (i < num_threads)
	{
		pthread_join(philosophers[i].thread_id, NULL);
		++i;
	}
}

static t_status	malloc_resources(t_philo_config *config,
							t_fork **forks,
							t_philo **philosophers,
							t_display *display)
{
	*display = create_display();
	if (!display->lock)
		return (ERROR);
	*forks = create_forks(config->number_of_philosophers);
	if (!(*forks))
	{
		free(display->lock);
		return (ERROR);
	}
	*philosophers = create_philosophers(config, *forks, display);
	if (!(*philosophers))
	{
		free(display->lock);
		destroy_forks(*forks, config->number_of_philosophers);
		return (ERROR);
	}
	return (SUCCESS);
}

static t_status	run(t_philo_config *config)
{
	t_display	display;
	t_fork		*forks;
	t_philo		*philosophers;
	t_status	ret;

	ret = SUCCESS;
	forks = NULL;
	philosophers = NULL;
	if (malloc_resources(config, &forks, &philosophers, &display) == ERROR)
		return (ERROR);
	ret = create_philosophers_threads(philosophers, pthread_create);
	join_philosophers_threads(philosophers, config->initialized_threads);
	if (config->death_event)
		ret = DEATH_EVENT;
	cleanup(forks, philosophers, &display);
	return (ret);
}

int	main(int argc, const char *argv[])
{
	t_optional_philo_config	optional;
	t_status				ret;

	optional = parse_config_args(argc, argv);
	if (!optional.initialized)
	{
		display_usage_message();
		return (ERROR);
	}
	if (optional.config.number_of_philosophers < 2)
	{
		printf("Invalid Number of Philosophers\n");
		return (ERROR);
	}
	ret = run(&optional.config);
	return (ret);
}
