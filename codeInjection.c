#include "codeInjection.h"

int	main(int argc, char **argv)
{
	int			fd;
	Elf64_Addr	old_entry;
	Elf64_Ehdr	ehdr;

	(void)argc;
	fd = open_file(argv[1]);
	lseek(fd, 0, SEEK_SET);
	ehdr = modify_entrypoint(fd, &old_entry);
	InjectCode(fd, ehdr, old_entry);
	
	return 0;
}
