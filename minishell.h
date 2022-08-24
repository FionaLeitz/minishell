/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 10:25:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/24 17:19:16 by masamoil         ###   ########.fr       */
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

typedef enum s_sig_mode
{
	DEFAULT,
	HEREDOC,
	MUTE,
	COMMAND
}	t_sig_mode;

extern int	exit_st;

typedef struct s_fd
{
	int	a;
	int	b;
}		t_fd;

typedef union s_pipe_fd
{
	t_fd	frame;
	int		raw[2];
}			t_pipe_fd;

typedef struct s_export
{
	char			*name;
	char			*value;
	struct s_export	*next;
}					t_export;

typedef struct s_token
{	
	char			*value;
	char			**args;
	char			**red;
	int				fds[2];
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

typedef struct s_params
{
	char			**env;
	t_export		*export;
	t_data			*data;
}					t_params;

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
int			ft_execute(t_token *token, t_params *params);
void		ft_select_builtin(t_token *token, t_params *params, int i, int *old_fd);
int			ft_execute(t_token *token, t_params *params);
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
void		ft_sig_int(int signal);
void		ft_sig_quit(int signal);
void		ft_sig_heredoc(int signal);
void		ft_signals(t_sig_mode mode);
void		ft_exit_d(t_data *data, t_params *params);
void		check_exit_status(void);
//here_doc.c
const char	*hd_name(void);
int			ft_here_doc(char *str, t_params *params);
//int		ft_wait_hd(pid_t pid);
int			get_hd_line(char *del, int fd, int quotes, t_params *params);
char		*write_hd_expand(char *line, int fd, t_params *params);
//expand_heredoc.c
char		*rep_hd(t_params *params, char *str, int size, int quote);
char		*in_replace_hd(char *str, int s, char *line, int *i);
char		*expand_heredoc(char *line, t_params *params);
//heredoc_utils.c
int			ft_if_char(char *str, char c);
char		*ft_strncpy(char *dest, char *src, unsigned int n);
int			delim_quotes(char *delim);
int			check_delim(char *delim);
void			print_error_heredoc(char *str, int fd);
char		*del_quotes_hd(char *delim);
char		*ft_strjoin_char(char *s1, char c);
int			in_del_quote_hd(char *str, int i);
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
char		*rep(char **env, char *str, int *quote, char *buff);
int			in_replace(char *str, int s, t_token *token, t_data *data);
int			replace_var(t_token *token, t_data *data, t_params *params);
//redir_fd.c
int			get_fd_input(char *pathname, char *red);
int			get_fd_output(char *pathname, char *red);
void		ft_redirection(char **str, t_params *params, t_data *data, t_token *token);
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
int			check_fd(int fd);
int			check_child(int pid);
void			ft_shlvl(char **envp);

#endif

// ""''echo hola""'''' que""'' tal""''

// alias minishell='valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=.ignore_readline ./minishell'
// alias minishell='valgrind --track-fds=yes --suppressions=.ignore_readline ./minishell'
// {
// ignore_libreadline_conditional_jump_errors
// Memcheck:Leak
// ...
// obj:/usr/lib/x86_64-linux-gnu/libreadline.so.8.0
// }
