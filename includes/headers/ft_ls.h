#ifndef FT_LS_H
#define FT_LS_H

#include "../libft/libft.h"
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

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
t_file *create_tree(char *name, char *path);
void free_tree(t_file *file);
int add_child(t_file *parent, t_file *child);

// print.c
void print_ls(t_file **files, int n_files, t_options *options);
int recursive_print(t_file *file, t_options *options, int n_files, int depth);

// sort.c
int sort_ls(t_file **files, int n_files, t_options *options);


#endif