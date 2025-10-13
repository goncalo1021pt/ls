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
#include <limits.h>
#include <sys/ioctl.h>

// color codes
#define RESET       "\033[0m"
#define DIR_COLOR   "\033[01;34m"
#define LINK_COLOR  "\033[01;36m"
#define EXEC_COLOR  "\033[01;32m"
#define SOCK_COLOR  "\033[01;35m"
#define FIFO_COLOR  "\033[01;33m"
#define BLK_COLOR   "\033[01;33;44m"
#define CHR_COLOR   "\033[01;33;44m"
#define ORPHAN_COLOR "\033[01;31;40m"

typedef struct s_options {
	bool a;
	bool d;
	bool f;
	bool g;
	bool l;
	bool r;
	bool R;
	bool t;
	bool h;
	bool u;
	int n_args;
} t_options;

typedef struct s_file {
	char *name;
	char *path;
	char *printbuffer;
	struct stat stat;

	int n_children;
	struct s_file **children;
} t_file;

typedef struct s_widths {
	int nlink;
	int user;
	int group;
	int size;
} t_widths;


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
void set_color(t_file *file, t_options *options);
void extract_permissions(mode_t mode, char *permissions);

// print_lflag.c
void print_lflag(t_file *file, t_options *options, t_widths *w);
void get_column_widths(t_file **files, int n_files, t_widths *w, t_options *options);

// sort.c
int sort_ls(t_file **files, int n_files, t_options *options);


#endif