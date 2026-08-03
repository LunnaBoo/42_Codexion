/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luccribe <luccribe@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 02:44:23 by luccribe          #+#    #+#             */
/*   Updated: 2026/08/01 02:44:23 by luccribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <time.h>

typedef struct s_coder		t_coder;
typedef struct s_simulation	t_simulation;
typedef struct s_heap_node	t_heap_node;
typedef struct s_heap		t_heap;
typedef struct s_dongle		t_dongle;

typedef struct s_heap_node
{
	t_coder	*coder;
	long	priority;
	long	seq;
}	t_heap_node;

typedef int					(*t_heap_cmp)(t_heap_node *, t_heap_node *);

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			capacity;
	long		seq;
	t_heap_cmp	cmp;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	int				taken;
	long			available_after;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			*wait_queue;
}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread;
	int				id;
	int				is_compiling;
	int				compiling_score;
	long			last_compile;
	t_dongle		*r_dongle;
	t_dongle		*l_dongle;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*compile_lock;
	pthread_mutex_t	*print_lock;
	t_simulation	*sim;
}	t_coder;

typedef struct s_simulation
{
	pthread_t		thread;
	int				nb_of_coders;
	long			start_time;
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	compile_lock;
	pthread_mutex_t	print_lock;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				compiling_quota;
	long			dongle_cooldown;
	char			*scheduler;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_simulation;

struct timespec	ms_to_timespec(size_t milliseconds);
long			get_current_time(void);
int				ft_usleep(size_t milliseconds);
int				ft_isdigit(int c);
int				sim_init(t_simulation *sim);
void			sim_destroy(t_simulation *sim);
int				init_dongles(t_simulation *sim);
int				init_coders(t_simulation *sim);
void			destroy_dongles(t_simulation *sim, int count);
void			destroy_dongle_partial(t_dongle *d);
int				heap_init(t_heap *h, int capacity, t_heap_cmp cmp);
void			heap_destroy(t_heap *h);
int				heap_push(t_heap *h, t_heap_node item);
int				heap_pop(t_heap *h, t_heap_node *out);
int				cmp_node(t_heap_node *a, t_heap_node *b);
void			log_action(t_simulation *sim, int id, char *msg);
void			log_burnout(t_simulation *sim, int id);
void			request_dongle(t_dongle *d, t_coder *c, t_simulation *sim);
void			release_dongle(t_dongle *d, long cooldown);
void			take_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);
void			*coder_routine(void *arg);
void			*monitor_routine(void *arg);
int				sim_run(t_simulation *sim);

#endif
