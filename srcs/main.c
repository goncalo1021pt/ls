#include "ft_ls.h"

void init_options(t_options *options) {
	options->a = false;
	options->l = false;
	options->r = false;
	options->R = false;
	options->t = false;
	options->h = false;
}

void print_help() {
	ft_fprintf(1, "Usage: ft_ls [OPTION]... [FILE]...\n");
	ft_fprintf(1, "List information about the FILEs (the current directory by default).\n");
	ft_fprintf(1, "Sort entries alphabetically if none of -cftuSUX nor --sort is specified.\n");
	ft_fprintf(1, "\nOptions:\n");
	ft_fprintf(1, "  -a, --all            do not ignore entries starting with .\n");
	ft_fprintf(1, "  -l                   use a long listing format\n");
	ft_fprintf(1, "  -r                   reverse order while sorting\n");
	ft_fprintf(1, "  -R                   list subdirectories recursively\n");
	ft_fprintf(1, "  -t                   sort by time modified\n");
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

int execute_ls(int argc, char **argv, int i, t_options *options) {
	char *path = ".";
	int start;
	int exit_code = 0;

	start = i;
	do {
		if (argc != 1)
			path = argv[i];
		DIR *dir = opendir(path);
		struct dirent *entry;

		if (dir == NULL) {
			if (options->n_args > 1) {
				exit_code = 2;
				continue;
			}
			ft_fprintf(2, "ft_ls: cannot access %s: No such file or directory\n", path);
			return 2;
		}

		if (i != start) 
			ft_printf("\n");
		if (options->n_args > 1) {
			ft_printf("%s:\n", path);
		}
		while ((entry = readdir(dir)) != NULL) {
			if (options->a || entry->d_name[0] != '.') {
				ft_printf("%s\n", entry->d_name);
			}
		}
		closedir(dir);
	} while (++i < argc);
	return exit_code;
}

int main(int argc, char **argv)
{
	t_options options;
	int i = 1;

	init_options(&options);
	if (parse_options(argc, argv, &i, &options) != 0)
		return 2;
	check_option(options);
	return execute_ls(argc, argv, i, &options);
}
