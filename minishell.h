/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 10:25:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/02 17:34:44 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/headers/ft_printf.h"
# include "libft/headers/get_next_line.h"
# include "libft/headers/libft.h"
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>
# include <fcntl.h>

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# define PROMPT "Minishell> "

typedef struct s_export
{
	char			*name;
	char			*value;
	struct s_export	*next;
}					t_export;

typedef struct s_params
{
	char			**env;
	t_export		*export;
}					t_params;

typedef struct s_token
{	
	char			*value;
	char			**args;
	char			**red;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_data
{
	char			*input;
	char			*trimmed;
	int			i;
	int			fd_in;
	int			fd_out;
	struct s_token	*head;
}					t_data;

// BUILT-IN
// cd.c
int			ft_cd(char **arg, t_params *params);
// echo.c
int			ft_echo(char **arg);
// env.c
int			ft_env(char **arg, t_params *params);
int			new_env(char *arg, t_params *params);
// exit.c
int			ft_exit(char **arg);
// export.c
int			ft_export(char **arg, t_params *params);
t_export	*create_export(char **env);
// export2.c
void		*free_export(t_export *export);
t_export	*new_element(char *str);
void		print_export(t_params *params);
// get_env.c
char		**ft_get_env(char **envp);
// pwd.c
int			ft_pwd(char **arg);
// select_builtin.c
void		ft_select_builtin(t_token *token, t_params *params);
// table_utils.c
void		print_table(char **table);
void		free_table(char **table);
// unset.c
int			ft_unset(char **arg, t_params *params);

// PARSING
//init.c
void		init_data(t_data *data);
void		init_token(t_token *token);
//prompt.c
int			print_prompt(t_data *data, t_params *params);
char		*readline(const char *prompt);
//signaux.c
void		free_struct(t_data *data);
void		free_params(t_params *params);
void		sig_manage(int signal);
void		ft_exit_d(t_data *data, t_params *params);
//parse.c
void		jump_quotes(char *str, t_data *data);
int			check_string(t_data *data);
int			check_redir(t_data *data);
//fct_list.c
t_token		*new_node(char *value);
void		push_front(t_data *data, char *value);
void		push_back(t_data *data, char *value);
void		preview(t_data	*data);
int			ft_size(t_token *lst);
//redirect.c

//redir_fd.c
void	get_fd_input(char *pathname, char *red);
void	get_fd_output(char *pathname, char *red);
void	ft_redirection(char **str);
//quotes.c
int			get_next_quote(char quote, t_data *data);
int			check_quotes(t_data *data);
//args_start.c
int			first_pipe_cut(t_data *data);
int			ft_count_words(t_data *data, char *s);
int			create_tab(t_data *data, t_token *token);
void		del_quotes(t_token *token);
int			count_red(t_data *data, t_token *token);
//utils.c
int			ft_space(char c);
void		free_table(char **table);

#endif

// ""''echo hola""'''' que""'' tal""''

// alias minishell='valgrind --leak-check=full --show-leak-kinds=all --suppressions=.ignore_readline ./minishell'
// {
// ignore_libreadline_conditional_jump_errors
// Memcheck:Leak
// ...
// obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.0
// }
