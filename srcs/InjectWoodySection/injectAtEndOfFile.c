#include "codeInjection.h"

static void	updateProgramHeaderTable(int fd, off_t file_size, Elf64_Ehdr ehdr, int shellcode_len)
{
	Elf64_Phdr	phdr;
	int			ret = 1;
	uint32_t	type = 0;
	int			i = 0;

	lseek(fd, ehdr.e_phoff, SEEK_SET);
	while (ret && type != PT_NOTE)
	{
		ret = read(fd, &phdr, sizeof(phdr));
		type = phdr.p_type;
		i++;
	}
	if (type == PT_NOTE)
	{
		phdr.p_type = PT_LOAD;
		phdr.p_offset = file_size;
		phdr.p_vaddr = file_size/* + getImageBase(fd, ehdr)*/;
		//phdr.p_vaddr = 0x500000;
		phdr.p_paddr = phdr.p_vaddr;
		phdr.p_filesz = shellcode_len;
		phdr.p_memsz = shellcode_len;//more that p_filesz if contains a .bss section if uninitialized data
		phdr.p_flags = PF_R | PF_X;
		phdr.p_align = 0x1000;
		//lseek(fd, sizeof(phdr) * -1, SEEK_CUR);
		lseek(fd, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET);
		write(fd, &phdr, sizeof(phdr));
	}
}

static uint32_t	updateSectionHeaderTable(int fd, off_t file_size, Elf64_Ehdr ehdr, int shellcode_len)
{
	Elf64_Shdr	shdr;
	int			ret = 1;
	uint32_t	type = 0;
	int			i = 0;

	lseek(fd, ehdr.e_shoff, SEEK_SET);
	while (ret && type != SHT_NOTE)
	{
		ret = read(fd, &shdr, sizeof(shdr));
		type = shdr.sh_type;
		i++;
	}
	if (type == SHT_NOTE)
	{
		shdr.sh_type = SHT_PROGBITS;
		shdr.sh_addr = file_size/* + getImageBase(fd, ehdr)*/;
		shdr.sh_offset = file_size;
		shdr.sh_size = shellcode_len;
		shdr.sh_addralign = 16;
		shdr.sh_flags = SHF_ALLOC | SHF_EXECINSTR;
		//lseek(fd, sizeof(shdr) * -1, SEEK_CUR);
		lseek(fd, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET);
		write(fd, &shdr, sizeof(shdr));
		return shdr.sh_name;
	}
	return -1;
}

static void	write_updated_sections_name(int fd, Elf64_Shdr *shstrtab, \
				char *shstrtab_data, uint32_t index)
{
	uint32_t 	i = 0;
	uint32_t	woody_strlen;
	char		zeros[100];
	

	woody_strlen = strlen(".woody");
	memset(zeros, 0, sizeof(zeros));
	while (i < shstrtab->sh_size)
	{
		if (i == index)
		{
			write(fd, ".woody", woody_strlen);
			write(fd, zeros, strlen(&shstrtab_data[i]) - woody_strlen);
			i += strlen(&shstrtab_data[i]);
		}
		else
			write(fd, &shstrtab_data[i++], 1);
	}
	//write(fd, &zero, 1);
	//write(fd, &shstrtab_data[++i], 1);
	write(fd, zeros, 1);
}

static void	update_section(int fd, Elf64_Shdr *shstrtab, uint32_t sh_name)
{
	char		shstrtab_data[shstrtab->sh_size];
	uint32_t	index = 0;

	lseek(fd, shstrtab->sh_offset, SEEK_SET);
	read(fd, shstrtab_data, shstrtab->sh_size);
	while (index < shstrtab->sh_size)
	{
		if (index == sh_name)
		{
			lseek(fd, shstrtab->sh_offset, SEEK_SET);
			write_updated_sections_name(fd, shstrtab, shstrtab_data, index);
			return ;
		}
		if (!shstrtab_data[index])
			index++;
		else
			index += strlen(&shstrtab_data[index]);
	}
}

static void	update_shstrtab_section(int fd, Elf64_Ehdr ehdr, uint32_t sh_name)
{
	Elf64_Shdr	shdr[ehdr.e_shnum];

	lseek(fd, ehdr.e_shoff, SEEK_SET);
	read(fd, shdr, sizeof(shdr));
	update_section(fd, &shdr[ehdr.e_shstrndx], sh_name);
}

void injectAtEndOfFile(int fd, char *shellcode, int shellcode_len)
{
	Elf64_Ehdr	ehdr;
	off_t		file_size;
	uint32_t	sh_name;

	ehdr = get_elf_hdr(fd);
	file_size = get_file_size(fd);
	if (!shellcode)
		return;
	write(fd, shellcode, shellcode_len);
	updateProgramHeaderTable(fd, file_size, ehdr, shellcode_len);
	sh_name = updateSectionHeaderTable(fd, file_size, ehdr, shellcode_len);
	update_shstrtab_section(fd, ehdr, sh_name);
}
