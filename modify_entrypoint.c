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

Elf64_Ehdr	modify_entrypoint(int fd, off_t file_size, Elf64_Addr *old_entry)
{
	char		buffer[file_size];
	//t_offset	offset;
	Elf64_Ehdr	ehdr;

	read_file(fd, buffer, file_size);
	ehdr = get_elf_hdr(buffer);
	*old_entry = ehdr.e_entry;
	//offset = code_cave(buffer, file_size);
	ehdr.e_entry = offset.prev_head + getImageBase(fd, ehdr);
	printf("new entrypoint = %lx\n", ehdr.e_entry);
	write_file(fd, &ehdr);

	return ehdr;
}
