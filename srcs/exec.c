#include "ft_ls.h"

void clear_files(t_file **files, int n_files) {
	for (int ctd = 0; ctd < n_files; ctd++) {
		free_tree(files[ctd]);
	}
	free(files);
}

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
		if (!options->a && !options->f && entry->d_name[0] == '.')
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
			if (temp_code != 0) {
				free_tree(file);
				continue;
			}
		}
		free_tree(file);
		if (index < argc - 1)
			ft_printf("\n");
	} while (++index < argc);

	return exit_code;
}