# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 16:01:17 by bdevessi          #+#    #+#              #
#    Updated: 2018/12/20 15:39:46 by bdevessi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls
CFLAGS = -Wall -Werror -Wextra -Iincludes
CC = gcc

SRCS = 	src/main.c \
		src/args.c \
		src/list.c \
		src/utils.c \
		src/sort.c \
		src/collect.c \
		src/putf.c \
		src/io.c \
		src/errors.c \
		src/print.c \
		src/free.c \
		src/long_format.c \
		src/collect_aux.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re proto
