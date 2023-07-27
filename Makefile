# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cschmied <cschmied@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/18 13:51:14 by cschmied          #+#    #+#              #
#    Updated: 2023/06/18 13:51:14 by cschmied         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= server

VPATH		:= sources

SRCS		:= main.cpp Socket.cpp Server.cpp

BUILDDIR	:= build
OBJS		:= $(SRCS:%.cpp=$(BUILDDIR)/%.o)

CFLAGS		:= -Wall -Werror -Wextra -std=c++98 -pedantic

CC			:= c++

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: %.cpp $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY: clean all re fclean

clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

all: $(NAME)
