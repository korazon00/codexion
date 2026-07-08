
#include "codexion.h"

// void custum_usleep(sim, time_to_sleep)
// {
// 	while(should_stop(sim))
// 	{
// 		usleep(1000);
// 	}
// }

// take_bouth_dongles(sim);// befaour take bouthe the dongles should be available, and time to could down passed and hes is in the top of the queue.
// custum_usleep(time_to_compile);
// releas_dongles(sim);
// custum_usleep(time_to_debuge);
// custum_usleep(time_to_refactore);

void *coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;

	while (!sim->stop)
	{
		take_dongles(coder);
		
		//compiling
		coder->last_comp_start = get_time_ms();
		log_state(sim, coder->id, "is compiling");
		usleep(sim->args.time_to_compile * 1000);
		coder->compile_count ++;

		//release
		release_dongles(coder);

		//debuging
		log_state(sim, coder->id, "is debuging");
		usleep(sim->args.time_to_debug * 1000);

		//refactoring
		log_state(sim, coder->id, "is refactoring");
		usleep(sim->args.time_to_refactor * 1000);
	}
	return (NULL);
}

int init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;

		if (pthread_create(
			&sim->coders[i].thread,
			NULL,
			coder_routine,
			&sim->coders[i]) != 0)
			return 1;
		i++;
	}
	return 0;
}

int	join_threads(t_sim	*sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	return (0);
}
