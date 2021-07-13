#include <stdio.h>
#include <parser.h>
#include <dinner.h>
#include <types.h>
#include <display.h>
#include <semaphore.h>
#include <stdlib.h>

t_bool	create_forks_semaphore(int num_forks)
{
	sem_t	*semaphore;

	sem_unlink(SEM_NAME);
	semaphore = sem_open(SEM_NAME, SEM_FLAGS, SEM_PERMS, num_forks);
	if (semaphore == SEM_FAILED)
		return (FALSE);
	if (sem_close(semaphore) < 0)
	{
		sem_unlink(SEM_NAME);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	create_semaphore(int num_forks)
{
	if (!create_forks_semaphore(num_forks))
		return (FALSE);
	if (!create_display_semaphore())
		return (FALSE);
	return (TRUE);
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
	if (!create_semaphore(optional.config.number_of_philosophers))
		return (ERROR);
	ret = run(&optional.config);
	return (ret);
}
