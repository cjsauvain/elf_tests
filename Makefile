NAME = inject

SRCS =	codeInjection.c				\
		getTextSectionCodeCave.c	\
		file_operations.c			\
		get_elf_hdr.c				\
		modify_entrypoint.c			\
		InjectCode.c				\

OBJS_DIR = obj

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

HEADER = codeInjection.h

CC = gcc

CFLAGS = -Wall -Werror -Wextra -g

$(OBJS_DIR)/%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
