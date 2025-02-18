#ifndef ENTRYPOINT_H
# define ENTRYPOINT_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <elf.h>

typedef struct	s_offset
{
	off_t	prev_head;
	off_t	prev_end;
	off_t	current_head;
	off_t	current_end;
}	t_offset;

int 		open_file(char *path);
void    	read_file(int fd, char *buffer, int file_size);
off_t   	get_file_size(int fd);
t_offset    code_cave(char *buffer, off_t file_size);
Elf64_Ehdr  get_elf_hdr(char *buffer);
#endif
