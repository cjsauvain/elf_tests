#include "codeInjection.h"

int	main(int argc, char **argv)
{
	int	fd;

	(void)argc;
	fd = open_file(argv[1]);

	InjectCode(fd);
	
	return 0;
}
