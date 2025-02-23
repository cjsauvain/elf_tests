#include "codeInjection.h"

static off_t	find_sh_name(int fd, Elf64_Shdr shstrtab, char *sh_name_str)
{
	char		shstrtab_data[shstrtab.sh_size];
	uint64_t	i = 0;

	lseek(fd, shstrtab.sh_offset, SEEK_SET);
	read(fd, shstrtab_data, shstrtab.sh_size);
	while (i < shstrtab.sh_size)
	{
		if (!strcmp(&shstrtab_data[i], sh_name_str))
			return i;
		i += strlen(shstrtab_data + i) + 1;
	}
	return -1;
}

off_t	get_sh_name(int fd, Elf64_Ehdr ehdr, char *sh_name_str)
{
	uint64_t	i;
	Elf64_Shdr	shdr[ehdr.e_shnum];
	Elf64_Shdr	shstrtab;

	lseek(fd, ehdr.e_shoff, SEEK_SET);
	read(fd, shdr, sizeof(shdr));
	shstrtab = shdr[ehdr.e_shstrndx];
	i = find_sh_name(fd, shstrtab, sh_name_str);

	return i;
}
