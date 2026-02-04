/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:19:35 by amtan             #+#    #+#             */
/*   Updated: 2026/02/04 14:09:44 by amtan            ###   ########.fr       */
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
	int				forks_init_count;
	int				print_mtx_inited;
	int				state_mtx_inited;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	state_mtx;
	t_philo			*philos;
}	t_table;

/* parse.c */
int		parse_args(t_table *table, int argc, char **argv);

/* init.c */
int		init_table(t_table *table);
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

/* simulation */
int		start_simulation(t_table *table);
void	*philo_routine(void *arg);
int		monitor_loop(t_table *table);

/* log */
int		print_state(t_philo *philo, char *msg);
int		print_death(t_philo *philo);

/* utils.c */
int		error_msg(const char *msg);
int		ft_atol_strict(const char *s, long *out);
int		ft_atoi_strict(const char *s, int *out);

#endif
