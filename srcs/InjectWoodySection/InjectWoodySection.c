#include "codeInjection.h"

void	injectWoodySection(int fd, Elf64_Ehdr ehdr, char *shellcode)
{
	(void)shellcode; //REMOVE
	change_PHT_position(fd, &ehdr); // At the end of file
	/*add_new_PH_entry(fd, ehdr);
	add_new_segment(fd, ehdr);
	add_new_SH_entry(fd, ehdr);
	add_Woody_section(fd, ehdr, shellcode);*/
}
