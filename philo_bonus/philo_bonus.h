/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:19:35 by amtan             #+#    #+#             */
/*   Updated: 2026/02/23 17:49:15 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/types.h>

# define SEM_FORKS_NAME "/philo_forks"
# define SEM_ROOM_NAME "/philo_room"
# define SEM_PRINT_NAME "/philo_print"
# define SEM_END_NAME "/philo_end"
# define SEM_START_NAME "/philo_start"
# define SEM_MEAL_PREFIX "/philo_meal_"

# define EXIT_FULL 0
# define EXIT_DIED 1
# define EXIT_ERR  2

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	long			last_meal_ms;
	int				meals_eaten;
	unsigned int	think_phase;
	sem_t			*meal_sem;
	pthread_t		monitor;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int			philo_count;
	long		time_die;
	long		time_eat;
	long		time_sleep;
	int			must_eat_count;
	int			must_eat_enabled;
	long		start_ms;
	sem_t		*sem_forks;
	sem_t		*sem_room;
	sem_t		*sem_print;
	sem_t		*sem_end;
	sem_t		*sem_start;
	pid_t		*pids;
	t_philo		*philos;
}	t_table;

/* parse_bonus.c */
int		parse_args(t_table *table, int argc, char **argv);

/* init_bonus.c */
int		init_table(t_table *table);
void	destroy_all(t_table *table);

/* simulation_utils_bonus.c */
void	sim_kill_all(t_table *table, int created);
int		sim_wait_all(void);
int		sim_post_start_all(t_table *table);
void	sim_close_parent_meals(t_table *table, int created);
void	sim_close_start_sem(t_table *table, int created);

/* simulation_fork_bonus.c */
int		sim_fork_children(t_table *table, int *created);

/* simulation_wait_bonus.c */
int		sim_wait_until_end(t_table *table, int created);

/* sem_utils_bonus.c */
int		sem_wait_retry(sem_t *sem);
int		sem_post_retry(sem_t *sem);
int		sem_open_create(const char *name, unsigned int value, sem_t **out);
void	build_meal_name(int id, char *buf, int buf_size);

/* time_bonus.c */
int		now_ms(long *out);
int		since_start_ms(t_table *table, long *out);
int		ms_sleep(long ms);

/* log_bonus.c */
int		print_state(t_philo *philo, const char *msg);
int		print_death(t_philo *philo);
int		write_line(long ts, int id, const char *msg);

/* simulation_bonus.c */
int		start_simulation(t_table *table);

/* philo_process_bonus.c */
void	philo_process(t_philo *philo);

/* philo_loop_bonus.c */
int		philo_loop(t_philo *philo);
int		philo_sleep_think(t_philo *philo);

/* philo_take_bonus.c */
int		philo_take_forks(t_philo *philo);
int		philo_put_forks(t_philo *philo);

/* philo_eat_bonus.c */
int		philo_eat_step(t_philo *philo);

/* monitor_bonus.c */
void	*monitor_routine(void *arg);

/* utils_bonus.c */
int		error_msg(const char *msg);
int		ft_atol_strict(const char *s, long *out);
int		ft_atoi_strict(const char *s, int *out);

#endif
