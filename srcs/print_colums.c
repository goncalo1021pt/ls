#include "ft_ls.h"

int get_total_widths(t_file *parent, t_options *options) {
	int total_width = 0;
	int count = 0;
	
	for (int i = 0; i < parent->n_children; i++) {
		if (!options->a && ft_strncmp(parent->children[i]->name, ".", 1) == 0)
			continue;
		total_width += ft_strlen(parent->children[i]->name);
		count++;
	}
	
	if (count > 1)
		total_width += (count - 1) * 2;
	
	return total_width;
}

int count_visible_files(t_file *parent, t_options *options) {
	int count = 0;
	
	for (int i = 0; i < parent->n_children; i++) {
		if (!options->a && ft_strncmp(parent->children[i]->name, ".", 1) == 0)
			continue;
		count++;
	}
	
	return count;
}

static void calculate_column_widths(t_file **visible_files, int visible_count, t_column_info *col_info) {
	col_info->column_widths = malloc(sizeof(int) * col_info->num_columns);
	if (!col_info->column_widths)
		return;
	
	for (int i = 0; i < col_info->num_columns; i++)
		col_info->column_widths[i] = 0;
	
	for (int i = 0; i < visible_count; i++) {
		int col = i / col_info->num_rows;
		int filename_len = ft_strlen(visible_files[i]->name);
		
		if (filename_len > col_info->column_widths[col])
			col_info->column_widths[col] = filename_len;
	}
	
	for (int i = 0; i < col_info->num_columns; i++)
		col_info->column_widths[i] += 2;
}

static t_column_info calculate_columns(t_file *parent, t_options *options) {
	t_column_info col_info;
	col_info.column_widths = NULL;
	int visible_count = 0;
	int max_width = 0;
	
	for (int i = 0; i < parent->n_children; i++) {
		if (!options->a && ft_strncmp(parent->children[i]->name, ".", 1) == 0)
			continue;
		visible_count++;
		int len = ft_strlen(parent->children[i]->name);
		if (len > max_width)
			max_width = len;
	}
	
	col_info.max_width = max_width;
	
	if (visible_count == 0) {
		col_info.num_columns = 0;
		col_info.num_rows = 0;
		return col_info;
	}
	
	int column_width_with_space = max_width + 2;
	col_info.num_columns = (options->tty_width + 2) / column_width_with_space;
	
	if (col_info.num_columns < 1)
		col_info.num_columns = 1;
	
	if (col_info.num_columns > visible_count)
		col_info.num_columns = visible_count;
	
	col_info.num_rows = (visible_count + col_info.num_columns - 1) / col_info.num_columns;
	
	return col_info;
}

void print_columns(t_file *parent, t_options *options) {
	t_column_info col_info = calculate_columns(parent, options);
	
	if (col_info.num_columns <= 0 || col_info.num_rows <= 0)
		return;
	
	t_file **visible_files = malloc(sizeof(t_file *) * parent->n_children);
	if (!visible_files)
		return;
	
	int visible_idx = 0;
	for (int i = 0; i < parent->n_children; i++) {
		if (!options->a && ft_strncmp(parent->children[i]->name, ".", 1) == 0)
			continue;
		visible_files[visible_idx++] = parent->children[i];
	}
	
	calculate_column_widths(visible_files, visible_idx, &col_info);
	
	for (int row = 0; row < col_info.num_rows; row++) {
		for (int col = 0; col < col_info.num_columns; col++) {
			int file_idx = col * col_info.num_rows + row;
			
			if (file_idx >= visible_idx)
				continue;
				
			t_file *file = visible_files[file_idx];
			set_color(file, options);
			ft_printf("%s", file->name);
			if (options->f != true && options->is_tty)
				ft_printf("%s", RESET);
			
			if (col < col_info.num_columns - 1 && file_idx < visible_idx - 1) {
				int padding = col_info.column_widths[col] - ft_strlen(file->name);
				for (int p = 0; p < padding; p++)
					ft_printf(" ");
			}
		}
		ft_printf("\n");
	}
	
	if (col_info.column_widths)
		free(col_info.column_widths);
	free(visible_files);
}