#include "codeInjection.h"

/*void	injectIntoExistingSection(int fd, Elf64_Ehdr ehdr, char *shellcode)
{
	lseek(fd, ehdr.e_entry - getImageBase(fd, ehdr), SEEK_SET);
	write(fd, shellcode, sizeof(shellcode));
}*/
