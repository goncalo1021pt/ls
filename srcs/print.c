#include "ft_ls.h"

void extract_permissions(mode_t mode, char *permissions) {
	permissions[0] = (S_ISDIR(mode))  ? 'd' : (S_ISLNK(mode))  ? 'l' : (S_ISCHR(mode))  ? 'c' : (S_ISBLK(mode))  ? 'b' : (S_ISFIFO(mode)) ? 'p' : (S_ISSOCK(mode)) ? 's' : '-';
	permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
	permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
	permissions[3] = (mode & S_IXUSR) ? ((mode & S_ISUID) ? 's' : 'x') : ((mode & S_ISUID) ? 'S' : '-');
	permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
	permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
	permissions[6] = (mode & S_IXGRP) ? ((mode & S_ISGID) ? 's' : 'x') : ((mode & S_ISGID) ? 'S' : '-');
	permissions[7] = (mode & S_IROTH) ? 'r' : '-';
	permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
	permissions[9] = (mode & S_IXOTH) ? ((mode & S_ISVTX) ? 't' : 'x') : ((mode & S_ISVTX) ? 'T' : '-');
}

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

void print_lflag(t_file *file, t_options *options) {
	char permissions[11];
	ft_bzero(permissions, 11);
	extract_permissions(file->stat.st_mode, permissions);
	struct passwd *pw = getpwuid(file->stat.st_uid); 
	struct group *gr = getgrgid(file->stat.st_gid); 
	char *ctimebuf = ctime(&file->stat.st_mtime);
	char timebuf[20];
	ft_bzero(timebuf, 20);

	if (ctimebuf) {
		ft_strncpy(timebuf, ctimebuf + 4, 7);
		timebuf[7] = '\0';
		ft_strncat(timebuf, ctimebuf + 20, 4);
		timebuf[11] = '\0';
	}
	else 
		ft_strncpy(timebuf, "??? ?? ??:??", 20);
	
	ft_printf("%s ", permissions);
	ft_putlnbr_fd(file->stat.st_nlink, 1);
	ft_printf(" %s ", pw ? pw->pw_name : "UNKNOWN");
	if (options->g == false)
		ft_printf("%s ", gr ? gr->gr_name : "UNKNOWN");
	ft_putlnbr_fd(file->stat.st_size, 1);
	ft_printf(" %s ", timebuf);
	set_color(file, options);
	ft_printf("%s", file->name);
	ft_printf("%s", RESET);
	if (S_ISLNK(file->stat.st_mode)) {
        char link_target[PATH_MAX + 1];
        ssize_t len = readlink(file->path, link_target, PATH_MAX);
        if (len != -1) {
            link_target[len] = '\0';
            ft_printf(" -> ");
			ft_printf("%s", link_target);
        }
    }
}

int recursive_print(t_file *file, t_options *options, int n_files, int depth) {
	if (file == NULL)
		return 0;
	if ((n_files > 1 || options->R) && options->d == false) {
		ft_printf("%s:\n", file->path);
	}
	if (file != NULL) {
		if (options->l || options->g) {
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
				print_lflag(file->children[ctd2], options);
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
				ft_printf("\n");
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