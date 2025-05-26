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

int recursive_print(t_file *file, t_options *options, int n_files, int depth) {
	bool print = true;

	if (file == NULL)
		return 0;
	if (ft_strcmp(file->name, ".") == 0 || ft_strcmp(file->name, "..") == 0)
		print = false;
	if ((n_files > 1 || options->R) && options->d == false) {
		if (print)
			ft_printf("%s:\n", file->path);
	}
	if (file != NULL) {
		t_widths w;
		if (options->l || options->g) {
			get_column_widths(&file, 1, &w, options);

			long total_blocks = 0;
			for (int ctd2 = 0; ctd2 < file->n_children; ctd2++) {
				if (!options->a && file->children[ctd2]->name[0] == '.' && file->children[ctd2]->name[1] != '\0')
					continue;
				total_blocks += file->children[ctd2]->stat.st_blocks;
			}
			ft_printf("total "); 
			ft_putlnbr_fd(total_blocks / 2, 1);
			ft_printf("\n");
		}
		for (int ctd2 = 0; ctd2 < file->n_children; ctd2++) {
			if (options->l || options->g) {
				print_lflag(file->children[ctd2], options, &w);
				if (ctd2 != file->n_children - 1)
					ft_printf("\n");
			}
			else
			{
				set_color(file->children[ctd2], options);
				ft_printf("%s", file->children[ctd2]->name);
				ft_printf("%s", RESET);
				if (ctd2 != file->n_children - 1)
					ft_printf("  ");
			}
		}
		if (file->n_children > 0)
			ft_printf("\n");
	}
	if (options->R) {
		for (int ctd2 = 0; ctd2 < file->n_children; ctd2++) {
			if (file->children[ctd2] != NULL && S_ISDIR(file->children[ctd2]->stat.st_mode)) {
				if (print) {
					if (file->children[ctd2]->name[0] == '.' && file->children[ctd2]->name[1] == '\0')
						continue;
					if (file->children[ctd2]->name[0] == '.' && file->children[ctd2]->name[1] == '.' && file->children[ctd2]->name[2] == '\0')
						continue;
					ft_printf("\n");
				}
				recursive_print(file->children[ctd2], options, file->children[ctd2]->n_children, depth + 1);
			}
		}
	}
	return 0;
}

void print_ls(t_file **files, int n_files, t_options *options) {
	for (int ctd = 0; ctd < n_files; ctd++) {
		recursive_print(files[ctd], options, n_files, 0);
	}
}