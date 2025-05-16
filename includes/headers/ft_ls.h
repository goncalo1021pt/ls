#ifndef FT_LS_H
#define FT_LS_H

#include "../libft/libft.h"
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>

typedef struct s_options {
	bool a;
	bool l;
	bool r;
	bool R;
	bool t;
	bool h;
	int n_args;
} t_options;

int parse_options(int argc, char **argv, int *index ,t_options *options);
int check_option(t_options options);
void print_options(t_options *options);

#endif