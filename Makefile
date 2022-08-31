# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fleitz <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/20 09:33:04 by fleitz            #+#    #+#              #
#    Updated: 2022/08/11 09:45:19 by masamoil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

_END=$'\033[0m
_GREEN=$'\033[1;32m
_YELLOW=$'\033[1;33m
_ORANGE=$'\033[38;2;255;165;0m

NAME  = minishell

SRCS =	srcs/parsing/main.c				\
	srcs/parsing/prompt.c				\
	srcs/parsing/init.c					\
	srcs/parsing/signaux.c				\
	srcs/parsing/parse.c				\
	srcs/parsing/fct_list.c				\
	srcs/parsing/quotes.c				\
	srcs/parsing/args_start.c			\
	srcs/parsing/utils.c				\
	srcs/parsing/redirection.c			\
	srcs/parsing/redir_fd.c				\
	srcs/parsing/dollar_fct.c			\
	srcs/parsing/here_doc.c				\
	srcs/parsing/free.c					\
	srcs/parsing/new_heredoc.c		\
	srcs/parsing/heredoc_utils.c		\
	srcs/builtin/cd.c					\
	srcs/builtin/echo.c					\
	srcs/builtin/env.c					\
	srcs/builtin/execution.c			\
	srcs/builtin/exit.c					\
	srcs/builtin/export.c				\
	srcs/builtin/export2.c				\
	srcs/builtin/get_env.c				\
	srcs/builtin/pipes.c				\
	srcs/builtin/pwd.c					\
	srcs/builtin/select_builtin.c		\
	srcs/builtin/table_utils.c			\
	srcs/builtin/unset.c				\

OBJS	= ${SRCS:.c=.o}

HEADERS	= minishell.h

CC	= clang

CFLAGS	= -Wall -Wextra -Werror -g3

LIBFT	= -L libft -lft

all: 		lib ${NAME}

${NAME}: 	${OBJS}
			@echo "${_GREEN}----------------\nMINISHELL\n----------------${_END}"
			@echo "${_ORANGE}Objects MINISHELL created${_END}"
			@${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME} -l readline
			@echo "${_GREEN}MINISHELL compiled succesfully !${_END}"

.c.o:		
			@${CC} ${CFLAGS} -I${HEADERS} -c $< -o $@

clean:		
			@rm -f ${OBJS}
			@make -C libft clean
			@echo "${_YELLOW}Objects minishell cleaned !${_END}"

fclean:		clean
			@rm -f ${NAME}
			@make -C libft fclean
			@echo "${_YELLOW}${NAME} cleaned !${_END}"

re:			fclean all

lib:		
			@make -C libft

.PHONY :	all clean fclean re norm lib
