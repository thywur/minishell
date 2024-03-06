# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/08 19:17:58 by alermolo          #+#    #+#              #
#    Updated: 2024/03/06 16:21:10 by alermolo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#--variables-------------------------------------------------------------------#

NAME		=	minishell
DEBUG		=	no
BONUS		=	no

#--includes & libraries--------------------------------------------------------#

INC_DIR			=	includes/
B_INC_DIR		=	bonus/includes_bonus/
LIBFT_DIR		=	libft/
HEADERS 		=	includes/minishell.h

#--sources & objects-----------------------------------------------------------#

SRC_DIR		=		srcs/
B_SRC_DIR	=		bonus/srcs_bonus
OBJ_DIR		=		.objects
SOURCES 	=		srcs/exec.c			\
					srcs/error.c		\
					srcs/free.c			\
					srcs/cmd_handler.c		\
					srcs/cmd_parsing.c		\

#--flags-----------------------------------------------------------------------#

ifeq ($(BONUS), no)
CFLAGS		=	-Wall -Wextra -g3  -I $(LIBFT_DIR) -I $(INC_DIR) #-Werror
else
CFLAGS		=	-Wall -Wextra -I $(LIBFT_DIR) -I $(B_INC_DIR) #-Werror
endif

DFLAGS		=	-g3 -fsanitize=address

#--debug & define flags--------------------------------------------------------#

ifeq ($(DEBUG), yes)
CFLAGS 		+=	$(DFLAGS)
endif

#--libs------------------------------------------------------------------------#

LIBFT	=	$(LIBFT_DIR)/libft.a

#--objects---------------------------------------------------------------------#

ifeq ($(BONUS), no)
OBJECTS	=	$(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
else
OBJECTS	=	$(addprefix $(OBJ_DIR)/, $(SOURCES_BONUS:.c=.o))
endif

#--global rules----------------------------------------------------------------#

.DEFAULT_GOAL = all

#--compilation rules-----------------------------------------------------------#

all:
	$(MAKE) libs
	$(MAKE) $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	$(CC) $^ $(CFLAGS) $(LIBFT) -o $@

ifeq ($(BONUS), no)
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
else
$(OBJ_DIR)/%.o: %.c $(HEADERS_BONUS)
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
endif


#--libs, debugs & bonus--------------------------------------------------------#

libs:
	$(MAKE) -C $(LIBFT_DIR)

debug:
	$(MAKE) re DEBUG=yes

bonus:
	$(MAKE) BONUS=yes

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

.PHONY: all libs debug bonus re clean fclean
