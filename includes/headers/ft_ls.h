#ifndef FT_LS_H
#define FT_LS_H

#include "../libft/libft.h"
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct s_options {
	bool a;
	bool l;
	bool r;
	bool R;
	bool t;
	bool h;
	int n_args;
} t_options;

typedef struct s_file {
	char *name;
	char *path;
	struct stat stat;

	int n_children;
	struct s_file **children;
} t_file;

// parsing.c
int parse_options(int argc, char **argv, int *index ,t_options *options);
int check_option(t_options options);
void print_options(t_options *options);
void print_help(void);

// exec.c
int execute_ls(int argc, char **argv, int i, t_options *options);

// tree.c
t_file *create_tree(char *path, char *name);
void free_tree(t_file *file);
int add_child(t_file *parent, t_file *child);

#endif