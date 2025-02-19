#include "codeInjection.h"

static void	makeExecutable(int fd, Elf64_Shdr shdr)
{
	shdr.sh_flags |= 0x4;
	lseek(fd, sizeof(shdr) * -1, SEEK_CUR);
	write(fd, &shdr, sizeof(shdr));
}

void	makeSectionExecutable(int fd, Elf64_Ehdr ehdr)
{
	Elf64_Shdr	shdr;

	lseek(fd, ehdr.e_shoff, SEEK_SET);
	for (int i = 0; i < ehdr.e_shnum; i++)
	{
		read(fd, &shdr, sizeof(shdr));
		if (ehdr.e_entry > shdr.sh_addr && ehdr.e_entry < shdr.sh_addr + shdr.sh_size)
		{
			makeExecutable(fd, shdr);
			break;
		}
	}
}
