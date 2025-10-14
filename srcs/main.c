#include "ft_ls.h"

int main(int argc, char **argv) {
	t_options options;
	int start_index = 1;

	ft_bzero(&options, sizeof(t_options));
	if (parse_options(argc, argv, &start_index, &options) != 0)
		return 2;
	check_option(options);
	return execute_ls(argc, argv, start_index, &options);
}
