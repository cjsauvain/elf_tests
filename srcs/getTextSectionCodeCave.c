#include "codeInjection.h"

static off_t	get_size_offset(off_t head, off_t end)
{
	return end - head;
}

static void	update_offset_struct(t_offset *offset, int index)
{
	if (get_size_offset(offset->prev_head, offset->prev_end) < \
			get_size_offset(offset->current_head, offset->current_end))
	{
		offset->prev_head = offset->current_head;
		offset->prev_end = offset->current_end;
	}
	offset->current_head = index;
	offset->current_end = index;
}

static t_offset	get_cave(char *buffer, off_t sectionSize)
{
	t_offset	offset;
	int			i;

	i = 0;
	memset(&offset, 0, sizeof(offset));
	while (i < sectionSize)
	{
		if (buffer[i] == 0x00 && i % 2 == 0)//mettre un modulo plus pertinent ?
			update_offset_struct(&offset, i);
		else if (buffer[i] == 0x00)
			offset.current_end++;
		i++;
	}
	if (get_size_offset(offset.prev_head, offset.prev_end) < \
			get_size_offset(offset.current_head, offset.current_end))
	{
		offset.prev_head = offset.current_head;
		offset.prev_end = offset.current_end;
	}
	return offset;
}

static off_t	getCaveOffset(int fd, off_t sectionSize)
{
	char		buffer[sectionSize];
	t_offset	offset;

	read(fd, buffer, sectionSize);
	offset = get_cave(buffer, sectionSize);
	printf("new offset = %lx\n", offset.prev_head);

	return offset.prev_head;
}

static off_t	get_sh_name(int fd, Elf64_Shdr shstrtab)
{
	uint64_t	i;
	char		shstrtab_data[shstrtab.sh_size];

	lseek(fd, shstrtab.sh_offset, SEEK_SET);
	read(fd, shstrtab_data, shstrtab.sh_size);
	i = 0;
	while (i < shstrtab.sh_size)
	{
		if (!strcmp(&shstrtab_data[i], ".text"))
			return i;
		i += strlen(shstrtab_data + i) + 1;
	}
	return -1;
}

static off_t	getOffset(Elf64_Shdr *shdr, uint16_t shnum, uint32_t text_sh_name, off_t *sectionSize)
{
	for (int i = 0; i < shnum; i++)
	{
		if (shdr[i].sh_name == text_sh_name)
		{
			*sectionSize = shdr[i].sh_size;
			return shdr[i].sh_offset;
		}
	}
	return -1;
}

static off_t	getTextSectionOffset(int fd, Elf64_Ehdr ehdr, off_t *sectionSize)
{
	Elf64_Shdr	shdr[ehdr.e_shnum];
	Elf64_Shdr	shstrtab;
	off_t		text_sh_name;

	lseek(fd, ehdr.e_shoff, SEEK_SET);
	read(fd, &shdr, sizeof(shdr));
	shstrtab = shdr[ehdr.e_shstrndx];

	text_sh_name = get_sh_name(fd, shstrtab);
	if (text_sh_name == -1)
		return -1;
	return getOffset(shdr, ehdr.e_shnum, text_sh_name, sectionSize);
}

off_t	getTextSectionCodeCave(int fd, Elf64_Ehdr ehdr)
{
	off_t	textSectionOffset;
	off_t	sectionSize = 0;
	off_t	caveOffset;

	textSectionOffset = getTextSectionOffset(fd, ehdr, &sectionSize);
	if (textSectionOffset == -1)
		return -1;
	lseek(fd, textSectionOffset, SEEK_SET);
	caveOffset = getCaveOffset(fd, sectionSize);
	return caveOffset;
}
