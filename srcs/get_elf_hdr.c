#include "codeInjection.h"

Elf64_Ehdr	get_elf_hdr(int fd)
{
	Elf64_Ehdr	ehdr;

	lseek(fd, 0, SEEK_SET);
	read(fd, &ehdr, sizeof(ehdr));

	return ehdr;
}
