#include "codeInjection.h"

static void	get_PHT(int fd, Elf64_Ehdr *ehdr, Elf64_Phdr *phdr)
{
	memset(phdr, 0, ehdr->e_phnum * ehdr->e_phentsize);
	lseek(fd, ehdr->e_phoff, SEEK_SET);
	read(fd, phdr, ehdr->e_phnum * ehdr->e_phentsize);
}

static void	replace_PHT_by_zeros(int fd, Elf64_Ehdr *ehdr)
{
	char	zero = '\x00';

	lseek(fd, ehdr->e_phoff, SEEK_SET);
	for (int i = 0; i < ehdr->e_phentsize * ehdr->e_phnum; i++)
		write(fd, &zero, 1);
}

static void	put_PHT_at_end_of_file(int fd, Elf64_Ehdr *ehdr, Elf64_Phdr *phdr)
{
	off_t	new_PHT_offset;

	lseek(fd, 0, SEEK_SET);
	new_PHT_offset = lseek(fd, 0, SEEK_END);
	//write_zeros_for_new_section_entry();
	write(fd, phdr, ehdr->e_phnum * ehdr->e_phentsize);
	ehdr->e_phoff = new_PHT_offset;
	lseek(fd, 0, SEEK_SET);
	write(fd, ehdr, ehdr->e_ehsize);
}

void	change_PHT_position(int fd, Elf64_Ehdr *ehdr)
{
	Elf64_Phdr	phdr[ehdr->e_phnum];

	get_PHT(fd, ehdr, phdr);
	replace_PHT_by_zeros(fd, ehdr);
	put_PHT_at_end_of_file(fd, ehdr, phdr);
}
