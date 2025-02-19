#include "codeInjection.h"

int	open_file(char *path)
{
	int	fd;

	fd = open(path, O_RDWR);
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

void	write_file(int fd, Elf64_Ehdr *ehdr)
{
	lseek(fd, 0, SEEK_SET);
	if (write(fd, ehdr, sizeof(*ehdr)) == -1)
	{
		perror("write");
		close(fd);
		exit(-1);
	}
}
