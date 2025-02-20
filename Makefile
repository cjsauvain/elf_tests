#Executable Name
NAME = inject

#Directories
SRCS_DIR = srcs
OBJS_DIR = obj
INC_DIR  = inc

#Sources / Headers
SRC =	codeInjection.c				\
		getTextSectionCodeCave.c	\
		file_operations.c			\
		get_elf_hdr.c				\
		modify_entrypoint.c			\
		InjectCode.c				\

SRC_INJECT_WOODY_SECTION =	InjectWoodySection.c	\
							change_PHT_position.c	\

SRCS =	$(addprefix $(SRCS_DIR)/, $(SRC))											\
		$(addprefix $(SRCS_DIR)/InjectWoodySection/, $(SRC_INJECT_WOODY_SECTION))	\

HEADER = $(INC_DIR)/codeInjection.h

#Objects
OBJS = $(subst $(SRCS_DIR)/,,$(SRCS:%.c=$(OBJS_DIR)/%.o))

#Compilation
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g
INC_FOLDER = -I $(INC_DIR)

#Rules
$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) $(INC_FOLDER) -c $< -o $@

all: $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)/InjectWoodySection

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(OBJS) $(INC_FOLDER) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
