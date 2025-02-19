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

static t_offset	get_cave(char *buffer, off_t file_size)
{
	t_offset	offset;
	int			i;

	i = 0;
	memset(&offset, 0, sizeof(offset));
	while (i < file_size)
	{
		if (buffer[i] == 0x00 && i % 16 == 0)
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

t_offset	code_cave(char *buffer, off_t file_size)
{
	t_offset	offset;

	offset = get_cave(buffer, file_size);
	printf("new offset = %lx\n", offset.prev_head);

	return offset;
}
