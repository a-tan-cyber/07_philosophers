# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/27 23:17:02 by amtan             #+#    #+#              #
#    Updated: 2026/02/12 21:22:00 by amtan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	philo_bonus

CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
CFLAGS	+=	-pthread

SRCS	:=	main_bonus.c \
		  parse_bonus.c \
		  sems_bonus.c \
		  state_sem_bonus.c \
		  time_bonus.c \
		  log_bonus.c \
		  philo_bonus.c \
		  philo_actions_bonus.c \
		  utils_bonus.c
				
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
