# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 16:01:17 by bdevessi          #+#    #+#              #
#    Updated: 2018/12/10 17:37:55 by bdevessi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls
CFLAGS = -Wall -Werror -Wextra -Iincludes -g
CC = gcc

include src.mk

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile src.mk

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re proto
