#include "entrypoint.h"

int	open_file(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(1);
	}
	return fd;
}

void	read_file(int fd, char *buffer, int file_size)
{
	if (read(fd, buffer, file_size) == -1)
	{
		perror("read");
		close(fd);
		exit(1);
	}
}

off_t	get_file_size(int fd)
{
	off_t	file_size;

	lseek(fd, 0, SEEK_SET);
	file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	return file_size;
}
