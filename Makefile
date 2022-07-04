NAME  = minishell

SRCS =	srcs/builtin/cd.c						\
		srcs/builtin/pipes.c					\
		srcs/builtin/echo.c						\
		srcs/builtin/env.c						\
		srcs/builtin/pwd.c						\
		srcs/builtin/exit.c						\
		srcs/builtin/export.c					\
		srcs/builtin/export2.c					\
		srcs/builtin/unset.c					\
		srcs/builtin/get_env.c					\
		srcs/builtin/table_utils.c				\
		srcs/builtin/select_builtin.c			\
		srcs/builtin/main_test.c				\
		srcs/parsing/calls.c					\
		srcs/parsing/check_elements.c			\
		srcs/parsing/funnels.c					\
		srcs/parsing/get_calls.c				\
		srcs/parsing/get_elements.c				\
		srcs/parsing/get_funnels.c				\
		srcs/parsing/parsing_utils.c			\
		srcs/parsing/parsing.c					\
		srcs/parsing/str_utils.c				\
		srcs/parsing/table_utils.c				\

OBJS	= ${SRCS:.c=.o}

HEADERS	= minishell.h

CC	= clang

CFLAGS	= -Wall -Wextra -Werror -g3

LIBFT	= -L libft -lft

all: 		lib ${NAME}

${NAME}: 	${OBJS}
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME}

.c.o:		
			${CC} ${CFLAGS} -I${HEADERS} -c $< -o $@

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
