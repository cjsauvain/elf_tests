#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct	s_offset
{
	off_t	prev_head;
	off_t	prev_end;
	off_t	current_head;
	off_t	current_end;
}	t_offset;

off_t	get_size_offset(off_t head, off_t end)
{
	return end - head;
}

void	update_offset_struct(t_offset *offset, int index)
{
	if (offset->prev_head 
		&& get_size_offset(offset->prev_head, offset->prev_end) < \
			get_size_offset(offset->current_head, offset->current_end))
	{
		offset->prev_head = offset->current_head;
		offset->prev_end = offset->current_end;
	}
	offset->current_head = index;
	offset->current_end = index;
}

off_t	get_carve(char *buffer, off_t file_size)
{
	t_offset	offset;
	int			i;

	i = 0;
	memset(&offset, 0, sizeof(offset));
	while (i < file_size)
	{
		if (!buffer[i] && buffer[i - 1])
			update_offset_struct(&offset, i);
		else if (!buffer[i])
			offset.current_end++;
		i++;
	}
	return get_size_offset(offset.prev_head, offset.prev_end);
}

off_t	code_carve(int fd, off_t file_size)
{
	char	buffer[file_size];
	off_t	offset;

	if (read(fd, buffer, file_size) == -1)
	{
		perror("read");
		close(fd);
		exit(1);
	}
	offset = get_carve(buffer, file_size);
}

int	main(int argc, char **argv)
{
	int		fd;
	off_t	file_size;
	off_t	offset;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return 1;
	}
	lseek(fd, 0, SEEK_SET);
	file_size = lseek(fd, 0, SEEK_END);
	offset = code_carve(fd, file_size);

	printf("offset = 0x%.08lx\n", offset);

	return 0;
}
