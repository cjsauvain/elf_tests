#Executable Name
NAME = inject

#Directories
SRCS_DIR = srcs
OBJS_DIR = obj
INC_DIR  = inc

#Sources / Headers
SRC =	main.c				\
		file_operations.c	\
		get_elf_hdr.c		\
		modify_entrypoint.c	\
		get_sh_name.c		\
		getImageBase.c		\

SRC_INJECT_WOODY_SECTION =	getTextSectionCodeCave.c	\
							InjectCode.c				\
							injectAtEndOfFile.c			\
							injectIntoExistingSection.c	\

SRCS =	$(addprefix $(SRCS_DIR)/, $(SRC))											\
		$(addprefix $(SRCS_DIR)/InjectWoodySection/, $(SRC_INJECT_WOODY_SECTION))	\

HEADER = $(INC_DIR)/codeInjection.h

#Objects
OBJS = $(subst $(SRCS_DIR)/,,$(SRCS:%.c=$(OBJS_DIR)/%.o))

#Compilation
CC = clang
CFLAGS = -Wall -Werror -Wextra -g -std=c23 -Wno-unused-result
INC_FOLDER = -I $(INC_DIR)

#Rules
all: $(NAME)

test:
	clang -o test-dyna test.c
	nix-shell -p pkgs.glibc.static --command 'clang -static -o test-static test.c'

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) $(INC_FOLDER) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)/InjectWoodySection

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(OBJS) $(INC_FOLDER) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME) test-static test-dyna

re: fclean all

.PHONY: all clean fclean re test
