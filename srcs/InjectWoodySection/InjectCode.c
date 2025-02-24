#include "codeInjection.h"
#include <string.h>
#include <stddef.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	k;

	if (little && !*little)
		return ((char *)big);
	if (len && big && little)
	{
		i = 0;
		while (i < len)
		{
			k = 0;
			while (i + k < len)
			{
				if (little[k] != big[i + k])
					break ;
				k++;
				if (!little[k])
					return ((char *)big + i);
			}
			i++;
		}
	}
	return (NULL);
}

static void	Injection(int fd, char *shellcode, int shellcode_len)
{
	//off_t	offset;

	/*offset = getTextSectionCodeCave(fd, ehdr);
	if (offset > 0)
		CodeCaveCase(fd, ehdr, shellcode);*/
	//else
		injectAtEndOfFile(fd, shellcode, shellcode_len);
}

/*void	InjectCode(int fd)
{
	char	shellcode[] = {
			0x48, 0x31, 0xc0,
			0xbf, 0x01, 0x00, 0x00, 0x00,
			0x48, 0x8d, 0x35, 0x18, 0x00, 0x00, 0x00,
			0xba, 0x0e, 0x00, 0x00, 0x00,
			0xb8, 0x01, 0x00, 0x00, 0x00,
			0x0f, 0x05,									// DYNAMIC ENTRYPOINT
			0x48, 0x31, 0xd2,
			0x49, 0xb8, 0x60, 0x50, 0x55, 0x55, 0x55,
			0x55, 0x00, 0x00,
			0x41, 0xff, 0xe0,
			0x2e, 0x2e, 0x2e, 0x2e, 0x57,
			0x4f,
			0x4f,
			0x44, 0x59,
			0x2e, 0x2e, 0x2e, 0x2e, 0x0a,
	};

	//memcpy(shellcode + 4, &old_entry, 3);
	Injection(fd, shellcode, sizeof(shellcode));
}*/

#include <stdint.h>

void	InjectCode(int fd)
{
	Elf64_Ehdr	ehdr;

	lseek(fd, 0, SEEK_SET);
	read(fd, &ehdr, sizeof(ehdr));
//	char	shellcode[] = {
//			0x48, 0x31, 0xc0,
//			0xbf, 0x01, 0x00, 0x00, 0x00,
//			0x48, 0x8d, 0x35, 0x18, 0x00, 0x00, 0x00,
//			0xba, 0x0e, 0x00, 0x00, 0x00,
//			0xb8, 0x01, 0x00, 0x00, 0x00,
//			0x0f, 0x05,
//			0x48, 0x31, 0xd2,
//			0x4c, 0x8d, 0x04, 0x25, 0x50, 0x33, 0x00, //lea r8, [0x43b0 - 0x1060]
//			0x00,
//			//0x41, 0xb8, 0x60, 0x10, 0x00, 0x00, //mov r8, 0x1060
//			0x41, 0xff, 0xe0,
//			0x2e, 0x2e, 0x2e, 0x2e, 0x57,
//			0x4f,
//			0x4f,
//			0x44, 0x59,
//			0x2e, 0x2e, 0x2e, 0x2e, 0x0a,
//	};

	uint64_t orig_entry = 0x4016c0;
	uint64_t new_entry = (uint64_t)lseek(fd, 0, SEEK_END);
	if (new_entry == (uint64_t)-1) {
		perror("lseek");
		exit(1);
	}
	size_t shhdrs_size = sizeof(Elf64_Shdr) * ehdr.e_shnum;
	new_entry -= shhdrs_size;
	new_entry += getImageBase(fd, ehdr);

	printf("Original entrypoint: % X\n", orig_entry);
	printf("New entrypoint: % X\n", new_entry);

	char	shellcode[] = {
		#embed "shellcode.bin"
	};
	printf("shellcode: %llu\n", sizeof(shellcode));

	char	*start_addr = ft_strnstr(shellcode, "\x8A\x99\xAA\xBB\xCC\xDD\xEE\xFF\0", sizeof(shellcode));
	char	*woody_addr = ft_strnstr(shellcode, "\x11\x22\x33\x44\x55\x66\x77\x8B\0", sizeof(shellcode));
	if (!start_addr || !woody_addr) {
		fprintf(stderr, "Not found! (%p & %p)\n", start_addr, woody_addr);
		exit(1);
	}
	memcpy(start_addr, (char*)&orig_entry, sizeof(orig_entry));
	memcpy(woody_addr, (char*)&new_entry, sizeof(new_entry));

	int ptdr = open("shellcode-new.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!ptdr) {
		perror("open");
		exit(1);
	}
	if (write(ptdr, shellcode, sizeof(shellcode)) != sizeof(shellcode)) {
		perror("write");
		close(ptdr);
		exit(1);
	}
	close(ptdr);

	//memcpy(shellcode + 34, &old_entry, 3);
	Injection(fd, shellcode, sizeof(shellcode));
}
