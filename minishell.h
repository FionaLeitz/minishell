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
	char		*name;
	char		*value;
	struct s_export	*next;	
}t_export;

typedef struct s_params
{
	char		**env;
	t_export	*export;
}t_params;

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
char		**ft_select_builtin(char **arg, t_params *params);
// table_utils.c
void		print_table(char **table);
void		free_table2(char **table);
// unset.c
int			ft_unset(char **arg, char **env, t_export *export);

// Parsing

typedef struct s_token
{	
	char			*value;
	char			**args;
	char			**red;
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

/*typedef struct	s_builtin
{

}t_builtin;*/

//init.c
void		init_data(t_data *data);
void		init_token(t_token *token);
//prompt.c
int			print_prompt(t_data *data);
char		*readline(const char *prompt);
//signaux.c
void		sig_manage(int signal);
void		ft_exit_d(t_data *data);
//parse.c
int			check_string(t_data *data);
//fct_list.c
t_token		*new_node(char *value, size_t type);
void		push_front(t_data *data, char *value, size_t type);
void		push_back(t_data *data, char *value, size_t type);
void		preview(t_data	*data);
int			ft_size(t_token *lst);
//redirect.c
t_token		*l_redirect(t_data *data, t_token *token_list);
//quotes.c
int			get_next_quote(char quote, t_data *data);
int			check_quotes(t_data *data);
//args_start.c
int first_pipe_cut(t_data *data);
int	ft_count_words(t_data *data, char *s, char c);
int	create_tab(t_data *data, t_token *token);
int	del_quotes(t_token *token);
//utils.c
int	ft_check_whitespace(char c);

#endif


// ech'o' bonjour "bla | bla" | cat -e