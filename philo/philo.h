/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:19:35 by amtan             #+#    #+#             */
/*   Updated: 2026/02/20 20:15:36 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			last_meal_ms;
	int				meals_eaten;
	unsigned int	think_phase;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int				philo_count;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				must_eat_count;
	int				must_eat_enabled;
	long			start_ms;
	int				stop;
	int				fatal;
	int				forks_init_count;
	int				print_mtx_inited;
	int				stop_mtx_inited;
	int				state_mtx_inited;
	int				start_mtx_inited;
	int				ready_mtx_inited;
	int				ready_count;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	stop_mtx;
	pthread_mutex_t	state_mtx;
	pthread_mutex_t	start_mtx;
	pthread_mutex_t	ready_mtx;
	t_philo			*philos;
}	t_table;

typedef struct s_mon_eval
{
	int	ended;
	int	dead_id;
	int	print_death;
}	t_mon_eval;

/* parse.c */
int		parse_args(t_table *table, int argc, char **argv);

/* init.c */
int		init_table(t_table *table);

/* destroy.c */
void	destroy_all(t_table *table);

/* time.c */
int		now_ms(long *out);
int		since_start_ms(t_table *table, long *out);
int		ms_sleep(t_table *table, long ms);

/* state.c */
int		get_stop(t_table *table, int *out);
int		set_stop(t_table *table, int value);
int		get_last_meal(t_philo *philo, long *out);
int		set_last_meal(t_philo *philo, long value);
int		get_meals_eaten(t_philo *philo, int *out);

/* simulation.c */
int		start_simulation(t_table *table);

/* simulation_setup.c */
int		sim_setup(t_table *table, int *created, int *gate_locked);

/* monitor.c */
int		monitor_loop(t_table *table);
int		monitor_step(t_table *table, int *ended);

/* monitor_eval.c */
int		monitor_eval_locked(t_table *table, long now, t_mon_eval *out);

/* monitor_checks.c */
int		monitor_find_dead_locked(t_table *table, t_philo **out_dead,
			long now_ms);
int		monitor_all_full_locked(t_table *table, int *out_full);

/* monitor_wait.c */
int		monitor_idle_sleep(t_table *table);

/* philo_routine.c */
void	*philo_routine(void *arg);

/* philo_loop.c */
void	philo_loop(t_philo *philo);

/* philo_loop_sync.c */
int		philo_stop_check(t_table *table, int *out_stop);
int		philo_unlock_forks(t_table *table,
			pthread_mutex_t *first, pthread_mutex_t *second);

/* philo_loop_helpers.c */
int		philo_sleep_think(t_philo *philo);

/* philo_take_forks.c */
int		philo_take_forks(t_philo *philo,
			pthread_mutex_t **out_first, pthread_mutex_t **out_second);

/* philo_eat.c */
int		philo_eat_step(t_philo *philo);

/* fatal.c */
void	fatal_stop_no_lock(t_table *table);
void	fatal_stop_best_effort(t_table *table);
int		fatal_return(t_table *table);
int		fatal_return_no_lock(t_table *table);

/* log.c */
int		print_state(t_philo *philo, const char *msg);
int		write_line(long ts, int id, const char *msg);

/* utils.c */
int		error_msg(const char *msg);
int		ft_atol_strict(const char *s, long *out);
int		ft_atoi_strict(const char *s, int *out);

#endif
