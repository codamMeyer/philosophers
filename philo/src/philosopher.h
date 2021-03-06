#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include "types.h"
# include <display.h>
# include <pthread.h>
# include <fork.h>

t_philo			*create_philosophers(t_philo_config *philo_config, \
										t_fork *forks, \
										t_display *display);
t_status		create_philosophers_threads(t_philo *philosophers, \
										t_pthread_create create_thread);
t_bool			get_forks(t_philo *philo);
void			start_to_eat(t_philo *philo);
t_life_status	start_to_sleep(t_philo *philo);
void			start_to_think(t_philo *philo);

#endif