#include <time_utils.h>
#include <display.h>
#include <unistd.h>

t_time_ms	get_timestamp(void)
{
	const uint64_t	milli_factor = 1000;
	struct timeval	time;
	t_time_ms		timestamp;

	gettimeofday(&time, NULL);
	timestamp.value = (time.tv_sec * milli_factor) + (time.tv_usec / milli_factor);
	return (timestamp);
}

t_time_ms	get_timestamp_diff(t_time_ms start)
{
	const t_time_ms diff = {.value = get_timestamp().value - start.value};

	return (diff);
}

t_bool	is_dead(t_philo *philo)
{
	const t_time_ms	time_diff = get_timestamp_diff(philo->config->time_start);

	if ((time_diff.value - philo->last_meal.value) > philo->config->time_to_die.value)
	{
		display_action_message(time_diff.value, philo, DIED);
		philo->config->death_event = TRUE;
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_dinner_over(t_philo *philo)
{
	if (philo->config->min_meals && philo->config->need_to_finish_meals == 0)
		return (TRUE);
	if (is_dead(philo) || philo->config->death_event)
		return (TRUE);
	return (FALSE);
}

void	sleep_ms(t_time_ms sleep_ms)
{
	const uint64_t	mult_value = 1000;

	usleep(sleep_ms.value * mult_value);
}
