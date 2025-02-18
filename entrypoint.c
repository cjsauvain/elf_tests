#include "entrypoint.h"

void	modify_entrypoint(int fd, off_t file_size)
{
	char		buffer[file_size];
	t_offset	offset;
	Elf64_Ehdr	ehdr;

	read_file(fd, buffer, file_size);
	ehdr = get_elf_hdr(buffer);
	printf("start entrypoint = %lx\n", ehdr.e_entry);
	offset = code_cave(buffer, file_size);
	ehdr.e_entry = offset.prev_head;
	printf("new entrypoint = %lx\n", ehdr.e_entry);
}

int	main(int argc, char **argv)
{
	int			fd;
	off_t		file_size;

	(void)argc;
	fd = open_file(argv[1]);
	file_size = get_file_size(fd);
	modify_entrypoint(fd, file_size);
	
	return 0;
}
