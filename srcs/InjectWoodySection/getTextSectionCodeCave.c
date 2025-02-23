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

static off_t	getOffset(int fd, uint16_t e_shnum, Elf64_Off e_shoff, \
					uint32_t sh_name, off_t *sectionSize)
{
	Elf64_Shdr	shdr[e_shnum];

	lseek(fd, e_shoff, SEEK_SET);
	read(fd, &shdr, sizeof(shdr));
	for (int i = 0; i < e_shnum; i++)
	{
		if (shdr[i].sh_name == sh_name)
		{
			*sectionSize = shdr[i].sh_size;
			return shdr[i].sh_offset;
		}
	}
	return -1;
}

static off_t	getSectionOffset(int fd, Elf64_Ehdr ehdr, char *sh_name_str, off_t *sectionSize)
{
	off_t		sh_name;

	sh_name = get_sh_name(fd, ehdr, sh_name_str);
	if (sh_name == -1)
		return -1;
	return getOffset(fd, ehdr.e_shnum, ehdr.e_shoff, sh_name, sectionSize);
}

off_t	getTextSectionCodeCave(int fd, Elf64_Ehdr ehdr)
{
	off_t	textSectionOffset;
	off_t	sectionSize = 0;
	off_t	caveOffset;

	textSectionOffset = getSectionOffset(fd, ehdr, ".text", &sectionSize);
	if (textSectionOffset == -1)
		return -1;
	lseek(fd, textSectionOffset, SEEK_SET);
	caveOffset = getCaveOffset(fd, sectionSize);
	return caveOffset;
}
