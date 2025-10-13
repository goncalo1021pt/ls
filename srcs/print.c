#include "ft_ls.h"

void set_color(t_file *file, t_options *options) {
	if (options->f == true)
		return;
	if (S_ISDIR(file->stat.st_mode))
		ft_printf("%s", DIR_COLOR);
	else if (S_ISLNK(file->stat.st_mode)) {
		struct stat target_stat;
		if (stat(file->path, &target_stat) == -1)
			ft_printf("%s", ORPHAN_COLOR); 
		else
			ft_printf("%s", LINK_COLOR);
	}
	else if (S_ISREG(file->stat.st_mode) && file->stat.st_mode & S_IXUSR)
		ft_printf("%s", EXEC_COLOR);
	else if (S_ISSOCK(file->stat.st_mode))
		ft_printf("%s", SOCK_COLOR);
	else if (S_ISFIFO(file->stat.st_mode))
		ft_printf("%s", FIFO_COLOR);
	else if (S_ISBLK(file->stat.st_mode))
		ft_printf("%s", BLK_COLOR);
	else if (S_ISCHR(file->stat.st_mode))
		ft_printf("%s", CHR_COLOR);
}

static void print_header(t_file *file, bool should_print_header) {
	if (should_print_header)
		ft_printf("%s:\n", file->path);
}

static void print_total_blocks(t_file *file, t_options *options) {
	if (!(options->l || options->g))
		return;
	
	long total_blocks = 0;
	for (int i = 0; i < file->n_children; i++) {
		if (!options->a && ft_strncmp(file->children[i]->name, ".", 1) == 0)
			continue;
		total_blocks += file->children[i]->stat.st_blocks;
	}
	ft_printf("total %ld\n", total_blocks / 2);
}

static void print_file_entry(t_file *file, t_options *options, t_widths *w, bool is_last) {
	if (options->l || options->g) {
		print_lflag(file, options, w);
		if (!is_last)
			ft_printf("\n");
	} else {
		set_color(file, options);
		ft_printf("%s%s", file->name, RESET);
		if (!is_last)
			ft_printf("  ");
	}
}

static void print_directory_contents(t_file *file, t_options *options) {
	if (file->n_children == 0)
		return;
	
	t_widths w;
	if (options->l || options->g)
		get_column_widths(&file, 1, &w, options);
	
	print_total_blocks(file, options);
	
	for (int i = 0; i < file->n_children; i++) {
		bool is_last = (i == file->n_children - 1);
		print_file_entry(file->children[i], options, &w, is_last);
	}
	ft_printf("\n");
}

static bool should_recurse_into(t_file *file, t_options *options) {
	if (!options->R || !S_ISDIR(file->stat.st_mode))
		return false;
	if (ft_strncmp(file->name, ".", 2) == 0 || ft_strncmp(file->name, "..", 3) == 0)
		return false;
	return true;
}

int recursive_print(t_file *file, t_options *options, int n_files, int depth) {
	if (file == NULL)
		return 1;
	
	bool should_print_header = (n_files > 1 || options->R) && !options->d;
	
	print_header(file, should_print_header);
	print_directory_contents(file, options);
	
	if (options->R) {
		bool first_subdir = true;
		for (int i = 0; i < file->n_children; i++) {
			if (should_recurse_into(file->children[i], options)) {
				if (first_subdir && should_print_header) {
					ft_printf("\n");
					first_subdir = false;
				}
				recursive_print(file->children[i], options, file->children[i]->n_children, depth + 1);
			}
		}
	}
	
	return 0;
}

void print_ls(t_file **files, int n_files, t_options *options) {
	for (int i = 0; i < n_files; i++) {
		recursive_print(files[i], options, n_files, 0);
		if (i != n_files - 1)
			ft_printf("\n");
	}
}