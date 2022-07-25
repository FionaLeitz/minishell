/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 10:25:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/20 10:46:44 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "Minishel> "
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
# include <sys/types.h>
# include <fcntl.h>

typedef struct s_end
{
	char			*str;
	struct s_end	*next;
}			t_end;

typedef struct s_export
{
	char			*name;
	char			*value;
	struct s_export	*next;	
}t_export;

typedef struct s_params
{
	char			**env;
	t_export		*export;
}t_params;

typedef struct s_token
{	
	char			*value;
	char			**args;
	char			**red;
	char			**red_tab;
	struct s_token	*next;
	struct s_token	*prev;
}t_token;

typedef struct s_data
{
	char			*input;
	char			*trimmed;
	int				i;
	struct s_token	*head;
}t_data;

// cd.c
int			ft_cd(char **arg, char **env);
// echo.c
int			ft_echo(char **arg);
// env.c
int			ft_env(char **arg, char **env);
char		**new_env(char *arg, char **env);
// exit.c
int			ft_exit(char **arg);
// export.c
char		**ft_export(char **arg, char **env, t_export *export);
t_export	*create_export(char **env);
// export2.c
void		*free_export(t_export *export);
t_export	*new_element(char *str);
void		print_export(t_export *export);
// get_env.c
char		**ft_get_env(char **envp);
// pwd.c
int			ft_pwd(char **arg);
// select_builtin.c
char		**ft_select_builtin(t_token *token, t_params *params);
// table_utils.c
void		print_table(char **table);
void		free_table(char **table);
// unset.c
int			ft_unset(char **arg, char **env, t_export *export);

// Parsing

//init.c
void		init_data(t_data *data);
void		init_token(t_token *token);
//prompt.c
int			print_prompt(t_data *data, t_params *params);
char		*readline(const char *prompt);
//signaux.c
void		free_struct(t_data *data);
void		sig_manage(int signal);
void		ft_exit_d(t_data *data);
//parse.c
int			check_string(t_data *data);
int			check_redir(t_data *data);
//fct_list.c
t_token		*new_node(char *value);
void		push_front(t_data *data, char *value);
void		push_back(t_data *data, char *value);
void		preview(t_data	*data);
int			ft_size(t_token *lst);
//quotes.c
int			get_next_quote(char quote, t_data *data);
int			check_quotes(t_data *data);
//args_start.c
int			first_pipe_cut(t_data *data);
int			ft_count_words(t_data *data, char *s);
int			create_tab(t_data *data, t_token *token);
int			del_quotes(t_token *token);
int			count_red(t_data *data, t_token *token);
//utils.c
int			ft_check_whitespace(char c);
void		free_table(char **table);
//redirect.c
int	get_fd_read(char *pathname, char *red);
int	get_fd_open(char*pathname, char *red);
int	count_redir(char *value);
//char **red_tab(t_data *data, t_token *token);
int red_tab(t_data *data, t_token *token);

#endif

// ech' o' bonjour "bla | bla" | cat -e
// echo bla > test > test2
// echo bla > test2
