#include "types.h"
#include <display.h>
#include <philosopher.h>
#include <time_utils.h>

t_bool	get_forks(t_philo *philo)
{
	if (!philo->forks.left->is_taken && !philo->forks.right->is_taken)
	{
		philo->forks.left->is_taken = \
			pthread_mutex_lock(philo->forks.left->lock) == 0;
		philo->forks.right->is_taken = \
			pthread_mutex_lock(philo->forks.right->lock) == 0;
		philo->last_meal = get_timestamp_diff(philo->config->time_start);
		display_action_message(philo->last_meal.value, philo, HAS_TAKEN_A_FORK);
		display_action_message(philo->last_meal.value, philo, HAS_TAKEN_A_FORK);
		return (TRUE);
	}
	return (FALSE);
}

static void	drop_forks(t_forks_pair *forks)
{
	pthread_mutex_unlock(forks->left->lock);
	pthread_mutex_unlock(forks->right->lock);
	forks->left->is_taken = FALSE;
	forks->right->is_taken = FALSE;
}

void	start_to_eat(t_philo *philo)
{
	display_action_message(philo->last_meal.value, philo, EATING);
	sleep_ms(philo->config->time_to_eat);
	philo->finished_eating = get_timestamp_diff(philo->config->time_start);
	drop_forks(&philo->forks);
	if (philo->config->min_meals)
	{
		++(philo->meals_counter);
		if (philo->meals_counter == philo->config->min_meals)
			--(philo->config->need_to_finish_meals);
	}
}
