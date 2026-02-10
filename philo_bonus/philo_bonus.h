/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:19:35 by amtan             #+#    #+#             */
/*   Updated: 2026/02/10 22:41:56 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>

typedef struct s_table
{
	int		philo_count;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	int		must_eat_count;
	int		must_eat_enabled;
	long	start_ms;
	sem_t	*sem_forks;
	sem_t	*sem_room;
	sem_t	*sem_print;
	sem_t	*sem_state;
}	t_table;

typedef struct s_philo
{
	int			id;
	int			meals;
	long		last_meal_ms;
	int			done;
	pthread_t	monitor;
	t_table		*table;
}	t_philo;

/* parse_bonus.c */
int		parse_args(t_table *table, int argc, char **argv);

/* sems_bonus.c */
int		sems_open(t_table *table);
void	sems_close_unlink(t_table *table);
int		sem_wait_retry(sem_t *sem);

/* time_bonus.c */
int		now_ms(long *out);
int		ms_sleep(long ms);

/* log_bonus.c */
int		print_state(t_philo *philo, const char *msg);
int		print_death(t_philo *philo, long now);

/* philo_bonus.c */
void	philo_process(t_table *table, int id);

/* philo_actions_bonus.c */
int		take_forks(t_philo *philo);
int		put_forks(t_philo *philo);
int		eat_step(t_philo *philo);
int		sleep_think_step(t_philo *philo);

/* utils_bonus.c */
int		error_msg(const char *s);
int		ft_atol_strict(const char *s, long *out);
int		ft_atoi_strict(const char *s, int *out);

#endif
