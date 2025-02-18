#include "entrypoint.h"

Elf64_Ehdr	get_elf_hdr(char *buffer)
{
	Elf64_Ehdr	ehdr;

	memcpy(&ehdr, buffer, sizeof(ehdr));

	return ehdr;
}
