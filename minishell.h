/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 10:25:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/03 16:15:07 by masamoil         ###   ########.fr       */
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
	int				i;
	int				fd_in;
	int				fd_out;
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
int			syntax_check(t_data *data);
void		ft_cut(t_data *data, t_params *params);
int			print_prompt(t_data *data, t_params *params);
//free.c
void		free_struct(t_data *data);
void		free_params(t_params *params);
//signaux.c
void		sig_manage(int signal);
void		ft_exit_d(t_data *data, t_params *params);
//here_doc.c
char		*here_doc(char *str);
void		get_hd_line(char *del);
//parse.c
void		jump_quotes(char *str, t_data *data);
int			check_string(t_data *data);
int			first_pipe_cut(t_data *data);
//fct_list.c
t_token		*new_node(char *value);
void		push_front(t_data *data, char *value);
void		push_back(t_data *data, char *value);
int			ft_size(t_token *lst);
//dollar_fct.c
char		*rep(char **env, char *str, int size, int quote);
int			in_replace(char *str, int s, t_token *token, t_data *data);
int			replace_var(t_token *token, t_data *data, t_params *params);
//redir_fd.c
void		get_fd_input(char *pathname, char *red);
void		get_fd_output(char *pathname, char *red);
void		ft_redirection(char **str);
//redirection.c
char		first_redir(char *str, t_data *data);
int			check_redir(t_data *data);
int			get_red(t_data *data, t_token *token, int count);
int			count_red(t_data *data, t_token *token);
//quotes.c
int			get_next_quote(char quote, t_data *data);
int			check_quotes(t_data *data);
int			in_del_quote(char *str, int j);
void		del_quotes_redir(t_token *token);
void		del_quotes(t_token *token);
//args_start.c
int			ft_count_words(t_data *data, char *s);
int			in_create_tab(char *str, int *i);
int			create_tab(t_data *data, t_token *token);
//utils.c
int			ft_space(char c);

#endif

// ""''echo hola""'''' que""'' tal""''

// alias minishell='valgrind --leak-check=full --show-leak-kinds=all --suppressions=.ignore_readline ./minishell'
// {
// ignore_libreadline_conditional_jump_errors
// Memcheck:Leak
// ...
// obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.0
// }
