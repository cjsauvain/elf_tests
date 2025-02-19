#include "codeInjection.h"

int	main(int argc, char **argv)
{
	int			fd;
	off_t		file_size;
	Elf64_Addr	old_entry;
	Elf64_Ehdr	ehdr;

	(void)argc;
	fd = open_file(argv[1]);
	file_size = get_file_size(fd);
	lseek(fd, 0, SEEK_SET);
	ehdr = modify_entrypoint(fd, file_size, &old_entry);
	//makeSectionExecutable(fd, ehdr);
	InjectCode(fd, ehdr, old_entry);
	
	return 0;
}
