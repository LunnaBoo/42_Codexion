#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_coder
{
	pthread_t	thread;
	
}	t_coder

typedef struct s_simulation
{
	t_coder	*coders;
}	t_simulation

#endif
