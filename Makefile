
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
	srcs/builtin/cd.c					\
	srcs/builtin/echo.c					\
	srcs/builtin/env.c					\
	srcs/builtin/exit.c					\
	srcs/builtin/export.c				\
	srcs/builtin/export2.c				\
	srcs/builtin/get_env.c				\
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
			@echo "${_GREEN}----------------\nMINISHELL\n----------------${_END}"
			@echo "${_ORANGE}Objects MINISHELL created${_END}"
			@echo "${_GREEN}MINISHELL compiled succesfully !${_END}"

${NAME}: 	${OBJS}
			@${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME} -l readline

.c.o:		
			@${CC} ${CFLAGS} -I${HEADERS} -c $< -o $@

clean:		
			rm -f ${OBJS} 
			make -C libft clean

fclean:		clean
			rm -f ${NAME}
			make -C libft fclean

re:			fclean all

lib:		
			make -C libft

norm:	
			norminette -R CheckSourceHeader ${SRCS}
			norminette -R CheckDefine ${HEADERS}
			make -C libft norm

.PHONY :	all clean fclean re norm lib
