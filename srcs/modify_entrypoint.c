#include "codeInjection.h"

Elf64_Addr	getImageBase(int fd, Elf64_Ehdr ehdr)
{
	Elf64_Phdr	phdr;

	lseek(fd, ehdr.e_phoff, SEEK_SET);
	for (int i = 0; i <	ehdr.e_shnum; i++)
	{
		read(fd, &phdr, sizeof(phdr));
		if (phdr.p_type == PT_LOAD)
			break;
	}
	return phdr.p_vaddr;
}

Elf64_Ehdr	modify_entrypoint(int fd, Elf64_Addr *old_entry)
{
	Elf64_Ehdr	ehdr;

	ehdr = get_elf_hdr(fd);
	(void)old_entry; //REMOVE
	/**old_entry = ehdr.e_entry;
	ehdr.e_entry = offset + getImageBase(fd, ehdr);
	write_file(fd, &ehdr);*/

	return ehdr;
}
