#include "ft_ls.h"

/* int setup_files(t_file ***files, int *n_files, int start_index, int argc) {
	if (start_index == argc)
		*n_files = 1;
	else
		*n_files = argc - start_index;
	*files = malloc(sizeof(t_file *) * (*n_files));
	if (*files == NULL) {
		perror("malloc");
		return 2;
	}
	return 0;
} */

void clear_files(t_file **files, int n_files) {
	for (int ctd = 0; ctd < n_files; ctd++) {
		free_tree(files[ctd]);
	}
	free(files);
}

/* int tree_error(char *msg, DIR *dir) {
	perror(msg);
	if (dir != NULL)
		closedir(dir);
	return 2;
} */

/* int populate_tree(t_file *file, char *path, t_options *options, int depth) {
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
			if (child == NULL){
				free(full_path);
				return tree_error("create_tree", dir);
			}
			if (add_child(file, child) != 0)  {
				free(full_path);
				return tree_error("add_child", dir);
			}

			if (full_path == NULL) {
				free_tree(child);
				return tree_error("ft_strjoin", dir);
			}

			if (lstat(full_path, &child->stat) == -1) {
				free(full_path);
				free_tree(child);
				return tree_error("lstat", dir);
			}

			if (options->R && S_ISDIR(child->stat.st_mode)) {
				if (ft_strncmp(child->name, ".", 2) == 0) {
					free(full_path);
					continue;
				}
				if (ft_strncmp(child->name, "..", 3) == 0) {
					free(full_path);
					continue;
				}
				if (S_ISLNK(child->stat.st_mode)) {
					free(full_path);
					continue;
				}
				populate_tree(child, full_path, options, depth + 1);
			}	
			free(full_path);
		}
	}
	closedir(dir);
	return 0;
} */

/* int execute_ls_old(int argc, char **argv, int i, t_options *options) {
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
		} else {
			files[ctd] = create_tree(path, path);
			exit_code = populate_tree(files[ctd], path, options, 0);
		}

		if (exit_code != 0) {
			if (exit_code == 2) {
				if (!options->d) {
					free_tree(files[ctd]);
					files[ctd] = NULL;
				}
				continue;
			}
		} else {
			ctd++;
		}
	} while (++i < argc);

	exit_code = sort_ls_old(files, ctd, options);
	print_ls(files, ctd, options);
	clear_files(files, ctd);
	return exit_code;
} */

int execute_dflag(int argc, char **argv, int index, t_options *options) {
	t_file *parent = create_tree(".", ".");
	if (parent == NULL) {
		perror("create_tree");
		return 2;
	}

	int exit_code = 0;

	if (index == argc) {
		t_file *child = create_tree(".", ".");
		if (child == NULL) {
			free_tree(parent);
			return 2;
		}
		
		if (lstat(".", &child->stat) == -1) {
			ft_fprintf(2, "ft_ls: cannot access .: %s\n", strerror(errno));
			free_tree(child);
			free_tree(parent);
			return 2;
		}
		
		if (add_child(parent, child) != 0) {
			free_tree(child);
			free_tree(parent);
			return 2;
		}
	} else {
		for (int i = index; i < argc; i++) {
			t_file *child = create_tree(argv[i], argv[i]);
			if (child == NULL) {
				free_tree(parent);
				return 2;
			}
			
			if (lstat(argv[i], &child->stat) == -1) {
				ft_fprintf(2, "ft_ls: cannot access %s: %s\n", argv[i], strerror(errno));
				free_tree(child);
				exit_code = 2;
				continue; 
			}
			
			if (add_child(parent, child) != 0) {
				free_tree(child);
				free_tree(parent);
				return 2;
			}
		}
	}
	
	int sort_result = sort_ls(parent, options);
	if (sort_result > exit_code)
		exit_code = sort_result;
	
	print_ls(parent, options, false);
	
	free_tree(parent);
	return exit_code;
}

int load_dir_entries(t_file *file ,char *path, t_options *options) {
	struct dirent *entry;
	DIR *dir = opendir(path);
	if (dir == NULL) {
		ft_fprintf(2, "ft_ls: cannot access %s: %s\n", path, strerror(errno));
		return (errno == EACCES) ? 2 : 2; 
	}
	while ((entry = readdir(dir)) != NULL) {
		if (!options->a && entry->d_name[0] == '.')
			continue;
		
		char *new_path = ft_strjoin(path, "/");
		if (new_path == NULL) {
			closedir(dir);
			return 2;
		}
		char *full_path = ft_strjoin(new_path, entry->d_name);
		free(new_path);
		if (full_path == NULL) {
			closedir(dir);
			return 2;
		}
		t_file *child = create_tree(entry->d_name, full_path);
		if (child == NULL) {
			free(full_path);
			closedir(dir);
			return 2;
		}
		if (lstat(full_path, &child->stat) == -1) {
			free(full_path);
			free_tree(child);
			continue;
		}
		if (add_child(file, child) != 0) {
			free(full_path);
			free_tree(child);
			closedir(dir);
			return 2;
		}
		free(full_path);
	}
	closedir(dir);
	return 0;
}

static void set_exit_codes(int *exit_code, int tmp_code, int depth) {
	if (tmp_code != 0 && depth == 0)
		*exit_code = 2;
	else if (tmp_code != 0 && *exit_code != 2)
		*exit_code = 1;
}

int execute_tree(t_file *file, char *path, t_options *options, int depth, bool multiple_paths)  {
	int exit_code = 0;
	int tmp_code = 0;

	tmp_code = load_dir_entries(file, path, options);
	set_exit_codes(&exit_code, tmp_code, depth);

	tmp_code= sort_ls(file, options);
	set_exit_codes(&exit_code, tmp_code, depth);
	
	if (depth > 0)
		ft_printf("\n");
	print_ls(file, options, depth > 0 || multiple_paths || options->R);

	if (options->R) {
		for (int ctd = 0; ctd < file->n_children; ctd++) {
			if (file->children[ctd] != NULL && S_ISDIR(file->children[ctd]->stat.st_mode)) {
				if (ft_strncmp(file->children[ctd]->name, ".", 2) == 0)
					continue;
				if (ft_strncmp(file->children[ctd]->name, "..", 3) == 0)
					continue;
				if (S_ISLNK(file->children[ctd]->stat.st_mode))
					continue;
				tmp_code = execute_tree(file->children[ctd], file->children[ctd]->path, options, depth + 1, multiple_paths);
				free_tree(file->children[ctd]);
				file->children[ctd] = NULL;
				if (tmp_code > exit_code)
					exit_code = tmp_code;
			}
		}
	}
	return exit_code;
}

int execute_ls(int argc, char **argv, int index, t_options *options) {
	if (options->d)
		return execute_dflag(argc, argv, index, options);

	char *path = ".";
	int exit_code = 0;
	t_file *file = NULL;
	bool multiple_paths = (index < argc - 1);

	do {
		if (index != argc)
			path = argv[index];
		file = create_tree(path, path);
		if (file == NULL) {
			perror("create_tree");
			return 2;
		}
		else {
			int temp_code = execute_tree(file, path, options, 0, multiple_paths);
			if (temp_code > exit_code)
				exit_code = temp_code;

		}
		free_tree(file);
		if (index < argc - 1)
			ft_printf("\n");
	} while (++index < argc);

	return exit_code;
}