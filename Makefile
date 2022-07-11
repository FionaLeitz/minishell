NAME  = minishell

SRCS =	srcs/parsing/main.c\
	srcs/parsing/prompt.c\
	srcs/parsing/init.c\
	srcs/parsing/signaux.c\
	srcs/parsing/parse.c\
	srcs/parsing/fct_list.c\
	srcs/parsing/quotes.c\
	srcs/parsing/args_start.c\
	srcs/parsing/utils.c

OBJS	= ${SRCS:.c=.o}

HEADERS	= minishell.h

CC	= clang

CFLAGS	= -Wall -Wextra -Werror -g3

LIBFT	= -L libft -lft

all: 		lib ${NAME}

${NAME}: 	${OBJS}
			${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME} -l readline

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
