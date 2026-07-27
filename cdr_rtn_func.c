#include "codexion.h"

void compiling(t_sim *sim, t_coder *coder)
{
		pthread_mutex_lock(&coder->coder_mtx);
		coder->last_comp_start = get_time_ms();
		coder->compile_count ++;
		pthread_mutex_unlock(&coder->coder_mtx);
		log_state(sim, coder->id, "is compiling");
		custum_usleep(sim, sim->args.time_to_compile);
}

void debuging(t_sim *sim, t_coder *coder)
{
	log_state(sim, coder->id, "is debugging");
	custum_usleep(sim, sim->args.time_to_debug);
}

void refactoring(t_sim *sim, t_coder *coder)
{
	log_state(sim, coder->id, "is refactoring");
	custum_usleep(sim, sim->args.time_to_refactor);
}

void waiting_station(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mtx);
	pthread_cond_wait(&sim->cond, &coder->coder_mtx);
	pthread_mutex_unlock(&coder->coder_mtx);
}

void release_dongles_if_not_aval(t_sim *sim, t_coder *coder)
{
	if (!sim->dongles[coder->left].is_available &&
		!sim->dongles[coder->right].is_available)
		release_dongles(coder);
}
