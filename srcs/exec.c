#include "ft_ls.h"

int setup_files(t_file ***files, int *n_files, int i, int argc) {
	if (i == argc)
		*n_files = 1;
	else
		*n_files = argc - i;
	*files = malloc(sizeof(t_file *) * (*n_files));
	if (*files == NULL) {
		perror("malloc");
		return 2;
	}
	return 0;
}

void clear_files(t_file **files, int n_files) {
	for (int ctd = 0; ctd < n_files; ctd++) {
		free_tree(files[ctd]);
	}
	free(files);
}

void print_ls(t_file **files, int n_files, t_options *options) {
	(void)options;
	for (int ctd = 0; ctd < n_files; ctd++) {
		if (files[ctd] == NULL)
			continue;
		if (n_files > 1) 
			ft_printf("%s:\n", files[ctd]->name);
		if (files[ctd] != NULL) {
			for (int ctd2 = 0; ctd2 < files[ctd]->n_children; ctd2++) {
				ft_printf("%s", files[ctd]->children[ctd2]->name);
				if (ctd2 != files[ctd]->n_children - 1)
					ft_printf(" ");
			}
			if (files[ctd]->n_children > 0) 
				ft_printf("\n");
		}
	}
}

int execute_ls(int argc, char **argv, int i, t_options *options) {
	char *path = ".";
	int exit_code = 0;
	t_file **files = NULL;
	int n_files = 0;
	int ctd = 0;

	if (setup_files(&files, &n_files, i, argc) != 0)
		return 2;

	do {
		if (i != argc)
			path = argv[i];

		struct dirent *entry;
		DIR *dir = opendir(path);
		files[ctd] = NULL;

		if (dir == NULL) {
			exit_code = 2;
			if (options->n_args > 1) {
				ft_fprintf(2, "ft_ls: cannot access %s: No such file or directory\n", path);
				continue;
			}
		}
		
		files[ctd] = create_tree(path, path);
		while ((entry = readdir(dir)) != NULL) {
			if (!options->a && entry->d_name[0] == '.')
				continue;
			else {
				t_file *file = create_tree(path, entry->d_name);
				if (file == NULL) {
					perror("malloc");
					exit_code = 2;
					break;
				}
				if (add_child(files[ctd], file) != 0) {
					perror("add_child");
					exit_code = 2;
					break;
				}
			}
		}
		closedir(dir);
		ctd++;
	} while (++i < argc);
	print_ls(files, n_files, options);
	clear_files(files, n_files);
	return exit_code;
}
