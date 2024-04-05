# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2024/04/05 13:43:02 by alermolo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#--variables-------------------------------------------------------------------#

NAME		=	minishell
DEBUG		=	no
BONUS		=	no

#--includes & libraries--------------------------------------------------------#

INC_DIR			=	includes/
LIBFT_DIR		=	libft/
HEADERS 		=	includes/minishell.h

#--sources & objects-----------------------------------------------------------#

SRC_DIR		=		srcs/
OBJ_DIR		=		.objects
SOURCES 	=		srcs/error.c		\
					srcs/free.c			\
					srcs/exec/cmd_handler.c		\
					srcs/exec/cmd_parsing.c		\
					srcs/exec/exec.c			\
					srcs/exec/redirections.c	\
					srcs/exec/heredoc_utils.c	\
					srcs/builtins/exec_builtin.c	\
					srcs/builtins/builtin_utils.c	\
					srcs/builtins/cd.c	\
					srcs/builtins/echo.c	\
					srcs/builtins/env.c	\
					srcs/builtins/exit.c	\
					srcs/builtins/pwd.c	\
					srcs/builtins/export.c	\
					srcs/builtins/unset.c	\
					srcs/utils/utils_malloc.c \
					srcs/utils/utils_string.c \
					srcs/utils/utils_string2.c \
					srcs/utils/utils_token.c \
					srcs/utils/utils_free.c \
					srcs/utils/utils_redir.c \
					srcs/utils/utils.c \
					srcs/utils/ft_lstlast.c	\
					srcs/utils/ft_lstsize.c	\
					srcs/tokens/check_tokens.c \
					srcs/tokens/save_token.c \
					srcs/tokens/cmdline_to_tokens.c \
					srcs/tokens/tokens_expansion.c \
					srcs/blocks/join_tokens.c \
					srcs/signals/signals.c \
					main.c \

#--flags-----------------------------------------------------------------------#

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror -g3  -I $(LIBFT_DIR) -I $(INC_DIR)
LDFLAGS		=	-lreadline
DFLAGS		=	-g3 -fsanitize=address

#--debug & define flags--------------------------------------------------------#

ifeq ($(DEBUG), yes)
CFLAGS 		+=	$(DFLAGS)
endif

#--libs------------------------------------------------------------------------#

LIBFT	=	$(LIBFT_DIR)/libft.a

#--objects---------------------------------------------------------------------#

OBJECTS	=	$(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

#--global rules----------------------------------------------------------------#

.DEFAULT_GOAL = all

#--compilation rules-----------------------------------------------------------#

all:
	$(MAKE) libs
	$(MAKE) $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIBFT) -o $@

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#--libs, debugs & bonus--------------------------------------------------------#

libs:
	$(MAKE) -C $(LIBFT_DIR)

debug:
	$(MAKE) re DEBUG=yes

#--re, clean & fclean----------------------------------------------------------#

re:
	$(MAKE) fclean
	$(MAKE) all

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) -rf $(OBJECTS)

fclean:
	$(MAKE) clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

#--PHONY-----------------------------------------------------------------------#

.PHONY: all libs debug re clean fclean
