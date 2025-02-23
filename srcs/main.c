#include "codeInjection.h"

int	main(int argc, char **argv)
{
	int	fd;

	(void)argc;
	fd = open_file(argv[1]);
	modify_entrypoint(fd);
	InjectCode(fd);
	
	return 0;
}
