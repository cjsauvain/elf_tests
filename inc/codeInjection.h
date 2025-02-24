#ifndef ENTRYPOINT_H
# define ENTRYPOINT_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <elf.h>

# define	SHELLCODE_PATH	"injected_code/injected_code.o"
# define	ENTRYPOINT_POS	546

typedef struct	s_offset
{
	off_t	prev_head;
	off_t	prev_end;
	off_t	current_head;
	off_t	current_end;
}	t_offset;

int 		open_file(char *path);
void    	read_file(int fd, char *buffer, int file_size);
Elf64_Ehdr  get_elf_hdr(int fd);
void		modify_entrypoint(Elf64_Ehdr ehdr);
void		write_file(int fd, Elf64_Ehdr *ehdr);
off_t   	get_file_size(int fd);
void		InjectCode(int fd);
off_t		get_sh_name(int fd, Elf64_Ehdr ehdr, char *sh_name_str);
Elf64_Addr	getImageBase(int fd, Elf64_Ehdr ehdr);
off_t		getTextSectionCodeCave(int fd, Elf64_Ehdr ehdr);
void 		injectAtEndOfFile(int fd, char *shellcode, int shellcode_len);

#endif
