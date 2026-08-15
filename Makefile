# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inchoi <inchoi@student.42Seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/09 17:54:56 by inchoi            #+#    #+#              #
#    Updated: 2026/08/09 17:54:56 by inchoi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(strip $(HOSTTYPE)),)
override HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK := libft_malloc.so

CC := cc
CFLAGS := -Wall -Wextra -Werror -fPIC
CPPFLAGS := -Iincludes -I.
THREAD_FLAGS := -pthread
RM := rm -f

SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := libft
LIBFT := $(LIBFT_DIR)/libft.a

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
OS_SRC := utils_macos.c
SHARED_FLAGS := -dynamiclib
else ifeq ($(UNAME_S),Linux)
OS_SRC := utils_linux.c
SHARED_FLAGS := -shared
else
$(error Unsupported operating system: $(UNAME_S))
endif

SRCS := boxes.c \
		free.c \
		malloc.c \
		realloc.c \
		show_alloc_mem.c \
		show_alloc_mem_ex.c \
		support_debug.c \
		support_history.c \
		support_malloc.c \
		support_size.c \
		support_tags.c \
		support_thread.c \
		$(OS_SRC)

OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)
LIBFT_SRCS := $(filter-out %_bonus.c,$(wildcard $(LIBFT_DIR)/*.c)) \
			$(filter-out %_bonus.h,$(wildcard $(LIBFT_DIR)/*.h)) \
			$(LIBFT_DIR)/Makefile

all: $(NAME) $(LINK)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(SHARED_FLAGS) $(THREAD_FLAGS) -o $@ $(OBJS) $(LIBFT)

$(LINK): $(NAME)
	ln -sf $(NAME) $(LINK)

$(LIBFT): $(LIBFT_SRCS)
	$(MAKE) -C $(LIBFT_DIR) CFLAGS="-Wall -Wextra -Werror -fPIC"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c includes/ft_malloc.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(THREAD_FLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME) $(LINK)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
