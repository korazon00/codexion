
#include "codexion.h"

void custum_usleep(t_sim *sim, long time_to_sleep)
{
	long start;
	
	start = get_time_ms();
	while(!should_stop(sim) && get_time_ms() - start < time_to_sleep)
	{
		usleep((time_to_sleep / 20) * 1000);
	}
}

// take_bouth_dongles(sim);// befaour take bouthe the dongles should be available, and time to could down passed and hes is in the top of the queue.
// usleep(time_to_compile);
// releas_dongles(sim);
// usleep(time_to_debuge);
// usleep(time_to_refactore);

void *coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (!should_stop(sim))
	{
		take_dongles(coder);
		if (should_stop(sim))
		{
			release_dongles(coder);
			return NULL;
		}
		//compiling
		pthread_mutex_lock(&sim->sim_mtx);
		coder->last_comp_start = get_time_ms();
		coder->compile_count ++;
		pthread_mutex_unlock(&sim->sim_mtx);
		log_state(sim, coder->id, "is compiling");
		custum_usleep(sim, sim->args.time_to_compile);

		//release
		release_dongles(coder);
		log_state(sim, coder->id, "release dongles");

		//debuging
		log_state(sim, coder->id, "is debuging");
		custum_usleep(sim, sim->args.time_to_debug);

		//refactoring
		log_state(sim, coder->id, "is refactoring");
		custum_usleep(sim, sim->args.time_to_refactor);
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
		sim->dongles[i].id = i;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
		sim->coders[i].last_comp_start = sim->start_time;
		sim->coders[i].left = sim->coders[i].id - 1;
		sim->coders[i].right = sim->coders[i].id % sim->args.number_of_coders;

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
