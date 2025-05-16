#include "ft_ls.h"

void print_help() {
	ft_printf("Usage: ft_ls [OPTION]... [FILE]...\n");
	ft_printf("List information about the FILEs (the current directory by default).\n");
	ft_printf("Sort entries alphabetically if none of -cftuSUX nor --sort is specified.\n");
	ft_printf("\nOptions:\n");
	ft_printf("  -a,                  do not ignore entries starting with .\n");
	ft_printf("  -l                   use a long listing format\n");
	ft_printf("  -r                   reverse order while sorting\n");
	ft_printf("  -R                   list subdirectories recursively\n");
	ft_printf("  -t                   sort by time modified\n");
	exit(0);
}

int parse_options(int argc, char **argv, int *index ,t_options *options) {
	int i = 1;

	for (; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] != '\0') {
			for (int ctd = 1; argv[i][ctd] != '\0'; ctd++) {
				if (argv[i][ctd] == 'a')
					options->a = true;
				else if (argv[i][ctd] == 'l')
					options->l = true;
				else if (argv[i][ctd] == 'r')
					options->r = true;
				else if (argv[i][ctd] == 'R')
					options->R = true;
				else if (argv[i][ctd] == 't')
					options->t = true;
				else if (argv[i][ctd] == '-') {
					printf("options: %s\n", &argv[i][ctd]);
					if (ft_strnstr("-help", &argv[i][ctd], 6) != NULL) 
						options->h = true;
					break;
				}
				else {
					ft_fprintf(2, "ft_ls: invalid option -- '%c'\n", argv[i][ctd]);
					ft_fprintf(2, "Try 'ls --help' for more information.\n");
					return 2;
				}
			}
		}
		else
			break;
	}
	*index = i;
	options->n_args = argc - i;
	return 0;
}

void print_options(t_options *options) {
	if (options->a)
		ft_printf("Option a is set\n");
	if (options->l)
		ft_printf("Option l is set\n");
	if (options->r)
		ft_printf("Option r is set\n");
	if (options->R)
		ft_printf("Option R is set\n");
	if (options->t)
		ft_printf("Option t is set\n");
}

int check_option(t_options options) {
	if (options.h) {
		print_help();
		exit(0);
	}
	return 0;
}