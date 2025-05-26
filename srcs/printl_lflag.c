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

int count_digits_unsigned(unsigned long n) {
    int count = 1;
    while (n >= 10) {
        n /= 10;
        count++;
    }
    return count;
}

int count_digits_signed(long long n) {
    int count = (n < 0) ? 2 : 1; // 1 for digit, +1 for minus sign if negative
    if (n < 0) n = -n;
    while (n >= 10) {
        n /= 10;
        count++;
    }
    return count;
}

void ulong_to_str(unsigned long n, char *buf) {
    int len = count_digits_unsigned(n);
    buf[len] = '\0';
    while (len--) {
        buf[len] = '0' + (n % 10);
        n /= 10;
    }
}

void llong_to_str(long long n, char *buf) {
    int neg = n < 0;
    unsigned long long un = neg ? -n : n;
    int len = count_digits_signed(n);
    buf[len] = '\0';
    while (len--) {
        buf[len] = '0' + (un % 10);
        un /= 10;
    }
    if (neg) buf[0] = '-';
}

void print_padded_number(long n, int width)
{
	char buf[32];
	llong_to_str(n, buf);
	int len = ft_strlen(buf);
	for (int i = 0; i < width - len; i++)
		ft_putchar_fd(' ', 1);
	ft_putstr_fd(buf, 1);
}

void print_padded_string(char *s, int width)
{
	int len = ft_strlen(s);
	ft_putstr_fd(s, 1);
	for (int i = 0; i < width - len; i++)
		ft_putchar_fd(' ', 1);
}

void get_column_widths(t_file **files, int n_files, t_widths *w, t_options *options) {
	ft_bzero(w, sizeof(t_widths));
	for (int i = 0; i < n_files; i++) {
		t_file *file = files[i];
		for (int j = 0; j < file->n_children; j++) {
			t_file *child = file->children[j];
			if (!options->a && child->name[0] == '.' && child->name[1] != '\0')
				continue;
			int nlink_len = count_digits_unsigned((unsigned long)child->stat.st_nlink);
			if (nlink_len > w->nlink) w->nlink = nlink_len;
			struct passwd *pw = getpwuid(child->stat.st_uid);
			struct group *gr = getgrgid(child->stat.st_gid);
			int user_len = pw ? (int)ft_strlen(pw->pw_name) : 7;
			int group_len = gr ? (int)ft_strlen(gr->gr_name) : 7;
			if (user_len > w->user) w->user = user_len;
			if (group_len > w->group) w->group = group_len;
			int size_len = count_digits_signed((long long)child->stat.st_size);
			if (size_len > w->size) w->size = size_len;
		}
	}
}

static int is_current_year(time_t file_time) {
	struct tm *file_tm = localtime(&file_time);
	time_t now = time(NULL);
	struct tm *now_tm = localtime(&now);
	return (file_tm && now_tm && file_tm->tm_year == now_tm->tm_year);
}

void print_lflag(t_file *file, t_options *options, t_widths *w) {
	char permissions[11];
	ft_bzero(permissions, 11);
	extract_permissions(file->stat.st_mode, permissions);
	struct passwd *pw = getpwuid(file->stat.st_uid); 
	struct group *gr = getgrgid(file->stat.st_gid); 
	char *ctimebuf = ctime(&file->stat.st_mtime);
	char timebuf[20];
	ft_bzero(timebuf, 20);

	if (ctimebuf) {
		if (is_current_year(file->stat.st_mtime)) {
			ft_strncpy(timebuf, ctimebuf + 4, 7);
			timebuf[7] = '\0';
			ft_strncat(timebuf, ctimebuf + 11, 5);
			timebuf[12] = '\0';
		} else {
			ft_strncpy(timebuf, ctimebuf + 4, 7);
			timebuf[7] = '\0';
			ft_strncat(timebuf, ctimebuf + 20, 4);
			timebuf[11] = '\0';
		}
	}
	else 
		ft_strncpy(timebuf, "??? ?? ??:??", 20);

	ft_printf("%s ", permissions);
	print_padded_number((unsigned long)file->stat.st_nlink, w->nlink);
	ft_printf(" ");
	print_padded_string(pw ? pw->pw_name : "UNKNOWN", w->user);
	ft_printf(" ");
	if (options->g == false) {
		print_padded_string(gr ? gr->gr_name : "UNKNOWN", w->group);
		ft_printf(" ");
	}
	print_padded_number((long long)file->stat.st_size, w->size); 
	ft_printf(" ");
	ft_printf("%s ", timebuf);
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