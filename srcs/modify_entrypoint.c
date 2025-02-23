#include "codeInjection.h"

void	modify_entrypoint(int fd)
{
	Elf64_Ehdr	ehdr;

	ehdr = get_elf_hdr(fd);
	ehdr.e_entry = get_file_size(fd) + getImageBase(fd, ehdr);
	write_file(fd, &ehdr);
}
