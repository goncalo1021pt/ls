#include "ft_ls.h"

int main(int argc, char **argv) {
	t_options options;
	int i = 1;

	ft_bzero(&options, sizeof(t_options));
	if (parse_options(argc, argv, &i, &options) != 0)
		return 2;
	check_option(options);
	return execute_ls(argc, argv, i, &options);
}
