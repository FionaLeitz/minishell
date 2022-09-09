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

SRCS =	srcs/builtin/cd.c							\
		srcs/utils/create_export.c					\
		srcs/builtin/echo.c							\
		srcs/builtin/exit.c							\
		srcs/builtin/export.c						\
		srcs/builtin/export2.c						\
		srcs/builtin/env.c							\
		srcs/utils/get_env.c						\
		srcs/builtin/print.c						\
		srcs/builtin/pwd.c							\
		srcs/builtin/unset.c						\
		srcs/execution/execution.c 					\
		srcs/execution/pipes.c 						\
		srcs/execution/select_builtin.c 			\
		srcs/parsing/args_start.c					\
		srcs/parsing/dollar_fct.c					\
		srcs/parsing/parse.c 						\
		srcs/parsing/quotes.c 						\
		srcs/parsing/redirection.c 					\
		srcs/redirection/here_doc.c 				\
		srcs/redirection/heredoc_utils.c 			\
		srcs/redirection/new_heredoc.c 				\
		srcs/redirection/redir_fd.c 				\
		srcs/utils/fct_list.c 						\
		srcs/utils/free.c 							\
		srcs/utils/init.c 							\
		srcs/utils/table_utils.c 					\
		srcs/utils/utils.c 							\
		main.c 										\
		prompt.c 									\
		signals.c 									\

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
