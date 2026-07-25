/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 23:24:23 by ancheab           #+#    #+#             */
/*   Updated: 2026/07/19 23:24:24 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <time.h>

# define DONGLE_POLL_US 1000

typedef struct s_program	t_program;
typedef struct s_coder		t_coder;
typedef struct s_dongle		t_dongle;

typedef struct s_data
{
	int			number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	char		*scheduler;
}	t_data;

typedef struct s_request
{
	int		coder_id;
	long	key;
	long	order;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
	t_program	*program;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	t_heap			heap;
	int				in_use;
	long			last_release;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	long		last_compile_start;
	pthread_t	thread;
	t_program	*program;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
}	t_coder;

typedef struct s_program
{
	t_data			data;
	t_coder			*coders;
	t_dongle		*dongles;

	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	order_mutex;

	pthread_t		monitor;
	long			start_time;
	long			request_order;
	int				stop;
}	t_program;

int			error_msg(char *str);
int			is_numeric(char *str);
long		ft_atol(const char *str);
int			check_args(int argc, char **argv);
t_data		*init_data(char **argv);
int			init_program(t_program *prog);
long		get_time_ms(void);
void		build_timeout(struct timespec *ts, long ms_from_now);
void		*coder_routine(void *arg);
int			init_coders(t_program *prog);
void		destroy_program(t_program *prog);
int			create_threads(t_program *prog);
int			join_threads(t_program *prog);
void		print_status(t_program *prog, int id, char *msg);
int			take_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
void		*monitor_routine(void *arg);
int			simulation_stopped(t_program *prog);

int			heap_init(t_heap *heap, int capacity, t_program *prog);
void		heap_destroy(t_heap *heap);
void		heap_push(t_heap *heap, t_request req);
t_request	heap_pop(t_heap *heap);
t_request	heap_top(t_heap *heap);
void		heapify_up(t_heap *heap, int index);
void		heapify_down(t_heap *heap, int index);
void		heap_remove_by_id(t_heap *heap, int coder_id);
int			request_before(t_heap *heap, t_request a, t_request b);
int			allocate_memory(t_program *prog);

int			request_dongle(t_coder *coder, t_dongle *dongle, long order);
void		release_dongle(t_dongle *dongle);
long		next_request_order(t_program *prog);

#endif
