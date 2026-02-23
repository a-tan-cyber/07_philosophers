# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/27 23:17:02 by amtan             #+#    #+#              #
#    Updated: 2026/02/23 18:17:57 by amtan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	philo_bonus

CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
CFLAGS	+=	-pthread

SRCS	:= 	main_bonus.c parse_bonus.c utils_bonus.c init_bonus.c \
			destroy_bonus.c time_bonus.c log_bonus.c log_line_bonus.c \
			simulation_bonus.c simulation_utils_bonus.c simulation_fork_bonus.c \
			simulation_wait_bonus.c sem_utils_bonus.c philo_process_bonus.c \
			monitor_bonus.c philo_loop_bonus.c philo_loop_helpers_bonus.c \
			philo_take_bonus.c philo_eat_bonus.c
				
OBJS	:=	$(SRCS:.c=.o)

RM		:=	rm -f

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
