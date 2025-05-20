#include "ft_ls.h"

int compare_func(t_file *file1, t_file *file2, t_options *options) {
	if (options->t) {
		if (!options->r) {
			if (file1->stat.st_mtime != file2->stat.st_mtime)
				return file2->stat.st_mtime - file1->stat.st_mtime;
		} else {
			if (file1->stat.st_mtime != file2->stat.st_mtime)
				return file1->stat.st_mtime - file2->stat.st_mtime;
		}
	}
	if (options->r) {
		return ft_strcmp(file2->name, file1->name);
	}
	return ft_strcmp(file1->name, file2->name);
}

void insertion_sort(t_file *file, t_options *options) {
	
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

int merge_sort(t_file *file, t_options *options) {
    if (file->n_children < 2)
        return 0;

    int mid = file->n_children / 2;
    int n_left = mid;
    int n_right = file->n_children - mid;

    // Allocate left and right arrays
    t_file **left = malloc(sizeof(t_file *) * n_left);
	if (!left)
		return 2;
    t_file **right = malloc(sizeof(t_file *) * n_right);
	if (!right) {
		free(left);
		return 2;
	}

    // Copy data to left and right
    for (int i = 0; i < n_left; i++)
        left[i] = file->children[i];
    for (int i = 0; i < n_right; i++)
        right[i] = file->children[mid + i];

    // Create temp t_file to use insertion/merge recursively
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

    // Merge left and right back into file->children
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

int sort_args(t_file *file, t_options *options, int exit_code) {
	if (file == NULL || file->n_children <= 0)
		return exit_code;
	if (file->n_children  > 0 && file->n_children < 25) 
		insertion_sort(file, options);
	else 
		exit_code = merge_sort(file, options);
	for (int ctd = 0; ctd < file->n_children && options->R; ctd++) {
		if (file->children[ctd] != NULL && S_ISDIR(file->children[ctd]->stat.st_mode))
			sort_args(file->children[ctd], options, exit_code);
	}
	return exit_code;
}


int sort_ls(t_file **files, int n_files, t_options *options) {
	int exit_code = 0;
	
	if (files == NULL || n_files <= 0)
		return exit_code;
	for (int ctd = 0; ctd < n_files; ctd++) {
		exit_code = sort_args(files[ctd], options , exit_code);
	}
	return exit_code;
}