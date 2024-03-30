# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/08 16:11:24 by alermolo          #+#    #+#              #
#    Updated: 2024/03/30 16:25:59 by alermolo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 		libft.a

SRCS = 		ft_split.c	\
			ft_strjoin.c	\
			ft_strlen.c	\
			ft_strcmp.c	\
			ft_strncmp.c	\
			ft_substr.c	\
			get_next_line.c		\
			get_next_line_utils.c	\
			ft_arrlen.c	\
			ft_isdigit.c	\
			ft_isalpha.c	\
			ft_isalnum.c	\
			ft_strstr.c	\
			ft_itoa.c \
			ft_strchr.c	\

OBJ_DIR =	../.objects

OBJS =		$(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

HEADER =	libft.h

CC =		cc

CFLAGS =	-Wall -Werror -Wextra

RM =		rm -rf

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
				ar rcs $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:		clean
				$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
