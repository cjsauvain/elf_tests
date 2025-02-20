#include "codeInjection.h"

/*static void	injectIntoAlreadyExistingSection(int fd, Elf64_Ehdr ehdr, char *shellcode)
{
	lseek(fd, ehdr.e_entry - getImageBase(fd, ehdr), SEEK_SET);
	write(fd, shellcode, sizeof(shellcode));
}*/

static void	Injection(int fd, Elf64_Ehdr ehdr, char *shellcode)
{
	//off_t	offset;

	/*offset = getTextSectionCodeCave(fd, ehdr);
	if (offset > 0)
		injectIntoAlreadyExistingSection(fd, ehdr, shellcode);*/
	//else
		injectWoodySection(fd, ehdr, shellcode);
}

void	InjectCode(int fd, Elf64_Ehdr ehdr, Elf64_Addr old_entry)
{
	char	shellcode[] = {
			0x48, 0x31, 0xc0,
			0xb8, 0x00, 0x00, 0x00, 0x00,
			0xff, 0xe0
	};

	memcpy(shellcode + 4, &old_entry, 3);
	Injection(fd, ehdr, shellcode);
}

/*void	InjectCode(int fd, Elf64_Ehdr ehdr, Elf64_Addr old_entry)
{
	char	shellcode[] = {
			0x48, 0x31, 0xc0,
			0xbf, 0x01, 0x00, 0x00, 0x00,
			0x48, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00,
			0xba, 0x0b, 0x00, 0x00, 0x00,
			0xb8, 0x01, 0x00, 0x00, 0x00,
			0x0f, 0x05,
			0x48, 0x31, 0xc0,
			0xb8, 0x00, 0x00, 0x00, 0x00, //old_entry at position 34
			0xff, 0xe0,
			0x2e, 0x2e, 0x2e, 0x57,
			0x4f,
			0x4f,
			0x44, 0x59,
			0x2e,
			0x2e,
			0x2e,
	};

	memcpy(shellcode + 34, &old_entry, 3);
	lseek(fd, ehdr.e_entry - getImageBase(fd, ehdr), SEEK_SET);
	write(fd, shellcode, sizeof(shellcode));
}*/
