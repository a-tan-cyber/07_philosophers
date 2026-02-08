# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amtan <amtan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/27 23:17:02 by amtan             #+#    #+#              #
#    Updated: 2026/02/08 23:24:48 by amtan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	philo

CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
CFLAGS	+=	-pthread

SRCS	:=	main.c parse.c utils.c init.c time.c state.c log.c simulation.c \
				monitor.c monitor_lock.c monitor_checks.c monitor_wait.c \
				philo_routine.c philo_single.c philo_loop.c philo_take.c \
				philo_eat.c fatal.c 
				
OBJS	:=	$(SRCS:.c=.o)

RM		:=	rm -f

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
