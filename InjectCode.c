#include "codeInjection.h"
#include <stdio.h>

void	InjectCode(int fd, Elf64_Ehdr ehdr, Elf64_Addr old_entry)
{
	char	code[] = {
			0x48, 0x31, 0xc0,
			0xb8, 0x50, 0x16, 0x40, 0x00,
			0xff, 0xe0
	};

	printf("old entry: %0lX\n", old_entry);
//	memcpy(code + 4, &old_entry, 3);
	lseek(fd, ehdr.e_entry - getImageBase(fd, ehdr), SEEK_SET);
	write(fd, code, sizeof(code));
}
