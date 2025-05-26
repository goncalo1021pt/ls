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

int tree_error(char *msg, DIR *dir) {
	perror(msg);
	if (dir != NULL)
		closedir(dir);
	return 2;
}

int populate_tree(t_file *file, char *path, t_options *options, int depth) {
	struct dirent *entry;
	DIR *dir = opendir(path);
	if (dir == NULL) {
		ft_fprintf(2, "ft_ls: cannot access %s: No such file or directory\n", path);
		if (depth == 0)
			return 2;
		else
			return 1;
	}
	if (options->d) {
		t_file *child = create_tree(path, path);
		if (child == NULL)
			return tree_error("create_tree", dir);
		if (add_child(file, child) != 0)
			return tree_error("add_child", dir);
		if (lstat(path, &child->stat) == -1)
			return tree_error("lstat", dir);
		
	}
	else {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name[0] == '.' && !options->a)
				continue;
			char *new_path = ft_strjoin(path, "/");
			if (new_path == NULL) 
				return tree_error("ft_strjoin", dir);
			char *full_path = ft_strjoin(new_path, entry->d_name);
			free(new_path);

			t_file *child = create_tree(entry->d_name, full_path);
			if (child == NULL)
				return tree_error("create_tree", dir);
			if (add_child(file, child) != 0) 
				return tree_error("add_child", dir);

			if (full_path == NULL) 
				return tree_error("ft_strjoin", dir);
			if (lstat(full_path, &child->stat) == -1) 
				return tree_error("lstat", dir);

			if (options->R && S_ISDIR(child->stat.st_mode)) {
				if (entry->d_name[0] == '.' && entry->d_name[1] == '\0')
					continue;
				if (entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0')
					continue;
				populate_tree(child, full_path, options, depth + 1);
			}	
			free(full_path);
		}
	}
	closedir(dir);
	return 0;
}

int execute_ls(int argc, char **argv, int i, t_options *options) {
	char *path = ".";
	int exit_code = 0;
	t_file **files = NULL;
	int n_files = 0;
	int ctd = 0;

	if (setup_files(&files, &n_files, i, argc) != 0)
		return 2;
	if (options->d)
		n_files = 1;
	do {
		if (i != argc)
			path = argv[i];

		if (options->d) {
			if (ctd == 0)
				files[0] = create_tree(path, path);
			exit_code = populate_tree(files[0], path, options, 0);
		}
		else {
			files[ctd] = create_tree(path, path);
			exit_code = populate_tree(files[ctd], path, options, 0);
		}
		if (exit_code != 0) {
			if (exit_code == 2) {
				clear_files(files, n_files);
				return exit_code;
			}
			else
				continue;
		}
		ctd++;
	} while (++i < argc);
	
	exit_code = sort_ls(files, n_files, options);
	print_ls(files, n_files, options);
	clear_files(files, n_files);
	return exit_code;
}
