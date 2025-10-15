#include "ft_ls.h"

static int compare_words(const char *str1, const char *str2) {
	if (str1[0] == '.' && !(str1[1] == '\0' || (str1[1] == '.' && str1[2] == '\0'))) {
        str1++;
	}
    if (str2[0] == '.' && !(str2[1] == '\0' || (str2[1] == '.' && str2[2] == '\0'))) {
        str2++;
	}
	int diff_index = -1;
	const char *s1 = str1;
	const char *s2 = str2;

	while (*s1 && *s2) {
		int c1 = ft_tolower(*s1);
		int c2 = ft_tolower(*s2);
		if (c1 != c2)
			return c1 - c2;
		if (diff_index == -1 && *s1 != *s2)
			diff_index = s1 - str1;
		s1++;
		s2++;
	}
	if (*s1 || *s2)
		return (unsigned char)*s1 - (unsigned char)*s2;
	if (diff_index != -1) {
		char a = str1[diff_index];
		char b = str2[diff_index];
		if (ft_islower(a) && ft_isupper(b))
			return -1;
		if (ft_isupper(a) && ft_islower(b))
			return 1;
	}
	return 0;
}

static int compare_func(t_file *file1, t_file *file2, t_options *options) {
	if (options->t || (options->u && !options->l)) {
		time_t time1, time2;
		
		if (options->u) {
			time1 = file1->stat.st_atime;
			time2 = file2->stat.st_atime;
		} else {
			time1 = file1->stat.st_mtime;
			time2 = file2->stat.st_mtime;
		}
		
		if (!options->r) {
			if (time1 != time2)
				return time2 - time1;
		} else {
			if (time1 != time2)
				return time1 - time2;
		}
	}
	if (options->r) {
		return compare_words(file2->name, file1->name);
	}
	return compare_words(file1->name, file2->name);
}

static void insertion_sort(t_file *file, t_options *options) {
	
	for (int ctd = 1; ctd < file->n_children; ++ctd) {
		t_file *key = file->children[ctd];
		int ctd2 = ctd - 1;

		while (ctd2 >= 0 && compare_func(file->children[ctd2], key, options) > 0) {
			file->children[ctd2 + 1] = file->children[ctd2];
			ctd2--;
		}
		file->children[ctd2 + 1] = key;
	}
}

static int merge_sort(t_file *file, t_options *options) {
	if (file->n_children < 2)
		return 0;

	int mid = file->n_children / 2;
	int n_left = mid;
	int n_right = file->n_children - mid;

	t_file **left = malloc(sizeof(t_file *) * n_left);
	if (!left)
		return 2;
	t_file **right = malloc(sizeof(t_file *) * n_right);
	if (!right) {
		free(left);
		return 2;
	}

	for (int i = 0; i < n_left; i++)
		left[i] = file->children[i];
	for (int i = 0; i < n_right; i++)
		right[i] = file->children[mid + i];

	t_file left_file = { .children = left, .n_children = n_left };
	t_file right_file = { .children = right, .n_children = n_right };

	if (n_left < 25)
		insertion_sort(&left_file, options);
	else
		merge_sort(&left_file, options);

	if (n_right < 25)
		insertion_sort(&right_file, options);
	else
		merge_sort(&right_file, options);

	int l = 0, r = 0, k = 0;
	while (l < n_left && r < n_right) {
		if (compare_func(left[l], right[r], options) <= 0)
			file->children[k++] = left[l++];
		else
			file->children[k++] = right[r++];
	}
	while (l < n_left)
		file->children[k++] = left[l++];
	while (r < n_right)
		file->children[k++] = right[r++];

	free(left);
	free(right);
	return 0;
}

static int sort_dir(t_file *file, t_options *options, int exit_code) {
	if (file == NULL || file->n_children <= 0)
		return exit_code;
	if (file->n_children  > 0 && file->n_children < 25) 
		insertion_sort(file, options);
	else 
		exit_code = merge_sort(file, options);
	return exit_code;
}

int sort_ls(t_file *file, t_options *options) {
	int exit_code = 0;
	
	if (options->f)
		return exit_code;
	exit_code = sort_dir(file, options , exit_code);
	return exit_code;
}