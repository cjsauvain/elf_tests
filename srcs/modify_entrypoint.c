#include "codeInjection.h"

void	modify_entrypoint(Elf64_Ehdr ehdr)
{
	ehdr.e_entry = get_file_size(fd) + getImageBase(fd, ehdr);
	write_file(fd, &ehdr);
}
