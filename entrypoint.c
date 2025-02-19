#include "entrypoint.h"

int	main(int argc, char **argv)
{
	int			fd;
	off_t		file_size;
	Elf64_Ehdr	ehdr;

	(void)argc;
	fd = open_file(argv[1]);
	file_size = get_file_size(fd);
	ehdr = modify_entrypoint(fd, file_size);
	makeSectionExecutable(fd, ehdr);
	InjectCode(fd, ehdr);
	
	return 0;
}
