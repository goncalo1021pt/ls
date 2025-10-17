#include "ft_ls.h"

t_file *create_tree(char *name, char *path) {
	t_file *file = malloc(sizeof(t_file));
	if (file == NULL) {
		perror("malloc");
		return NULL;
	}
	file->name = ft_strdup(name);
	if (file->name == NULL) {
		perror("ft_strdup");
		free(file);
		return NULL;
	}
	file->path = ft_strdup(path);
	if (file->path == NULL) {
		perror("ft_strdup");
		free(file->name);
		free(file);
		return NULL;
	}
	file->n_children = 0;
	file->children_capacity = 0;
	file->children = NULL;
	return file;
}

void free_tree(t_file *file) {
	if (file == NULL)
		return;
	free(file->name);
	free(file->path);
	for (int i = 0; i < file->n_children; i++) {
		free_tree(file->children[i]);
	}
	free(file->children);
	free(file);
}

int add_child(t_file *parent, t_file *child) {
	if (parent == NULL || child == NULL)
		return -1;
	
	// Check if we need to grow the children array
	if (parent->n_children >= parent->children_capacity) {
		int new_capacity = parent->children_capacity == 0 ? 16 : parent->children_capacity * 2;
		t_file **new_children = realloc(parent->children, sizeof(t_file *) * new_capacity);
		if (new_children == NULL) {
			perror("realloc");
			return -1;
		}
		parent->children = new_children;
		parent->children_capacity = new_capacity;
	}
	
	parent->children[parent->n_children] = child;
	parent->n_children++;
	return 0;
}