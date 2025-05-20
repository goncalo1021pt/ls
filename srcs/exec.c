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

void extract_permissions(mode_t mode, char *permissions) {
	    permissions[0] = (S_ISDIR(mode))  ? 'd' :
                     (S_ISLNK(mode))  ? 'l' :
                     (S_ISCHR(mode))  ? 'c' :
                     (S_ISBLK(mode))  ? 'b' :
                     (S_ISFIFO(mode)) ? 'p' :
                     (S_ISSOCK(mode)) ? 's' : '-';

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

void print_lflag(t_file *file) {
	char permissions[11];
	ft_bzero(permissions, 10);
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
	// printf(" %s %s %ld %s %s\n", pw ? pw->pw_name : "UNKNOWN", gr ? gr->gr_name : "UNKNOWN", file->stat.st_size, timebuf, file->name);
	ft_printf(" %s %s ", pw ? pw->pw_name : "UNKNOWN", gr ? gr->gr_name : "UNKNOWN");
	ft_putlnbr_fd(file->stat.st_size, 1);
	ft_printf(" %s %s\n", timebuf, file->name);
}

void print_ls(t_file **files, int n_files, t_options *options) {
	(void)options;
	for (int ctd = 0; ctd < n_files; ctd++) {
		if (files[ctd] == NULL)
			continue;
		if (n_files > 1) 
			ft_printf("%s:\n", files[ctd]->name);
		if (files[ctd] != NULL) {
			if (options->l) {
                long total_blocks = 0;
                for (int ctd2 = 0; ctd2 < files[ctd]->n_children; ctd2++) {
                    if (lstat(files[ctd]->children[ctd2]->name, &files[ctd]->children[ctd2]->stat) == -1)
                        continue;
                    if (!options->a && files[ctd]->children[ctd2]->name[0] == '.')
                        continue;
                    total_blocks += files[ctd]->children[ctd2]->stat.st_blocks;
                }
                ft_printf("total "); 
				ft_putlnbr_fd(total_blocks / 2, 1);
				ft_printf("\n");
            }
			for (int ctd2 = 0; ctd2 < files[ctd]->n_children; ctd2++) {
				if (options->l) {
					print_lflag(files[ctd]->children[ctd2]);
				}
				else
				{
					ft_printf("%s", files[ctd]->children[ctd2]->name);
					if (ctd2 != files[ctd]->n_children - 1)
						ft_printf(" ");
				}
			}
			if (files[ctd]->n_children > 0) 
				ft_printf("\n");
		}
	}
}

void sort_args(t_file **files, int n_files, t_options *options) {
	(void)options;
	for (int ctd = 0; ctd < n_files; ctd++) {
		if (files[ctd] == NULL)
			continue;
		
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
	sort_args(files, n_files, options);
	print_ls(files, n_files, options);
	clear_files(files, n_files);
	return exit_code;
}
