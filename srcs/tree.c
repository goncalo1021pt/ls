#include "ft_ls.h"

t_file *create_tree(char *path, char *name) {
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
	file->path = ft_strjoin(path, name);
	if (file->path == NULL) {
		perror("ft_strjoin");
		free(file->name);
		free(file);
		return NULL;
	}
	file->n_children = 0;
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
	t_file **new_children = ft_realloc(parent->children, sizeof(t_file *) * parent->n_children, sizeof(t_file *) * (parent->n_children + 1));
	if (new_children == NULL) {
		perror("realloc");
		return -1;
	}
	parent->children = new_children;
	parent->children[parent->n_children] = child;
	parent->n_children++;
	return 0;
}