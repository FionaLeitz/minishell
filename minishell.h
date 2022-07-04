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

#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT "Minishell>"

typedef struct	s_export
{
	char			*name;
	char			*value;
	struct s_export	*next;	
}					t_export;

typedef struct	s_params
{
	char		**env;
	t_export	*export;
}				t_params;

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

typedef struct s_funnel {
	char	*type_funnel;
	char	**file_names;
}	t_funnel;

typedef struct s_call {
	char				*program;
	char				**args;
	t_funnel		**funnels;
}	t_call;

typedef struct s_data
{
        char    *input;
        char    *trimmed;
}t_data;

//my fcts
void	init_data(t_data *data);
int	print_prompt(t_data *data);
char	*readline(const char *prompt);

int	command(t_call **calls, t_params *params);

int count_chars(char c, char *str);
int check_quotes(char *str);
int	get_elements(char *input, int *n_elements, char ***table_elements);
void    free_table(char **table, int size);
void	print_table(char **table);
int	is_in(char c, char *str);
int	count_char_start(char c, char *str);
void	free_call(t_call call);
int get_size_table(char **table);
void    free_funnel(t_funnel *funnel);
void	print_funnel(t_funnel funnel);
void	print_call(t_call call);
void	print_calls(t_call **calls);
int	get_funnels(int *start, char **elements, t_call **call);
int table_dup(char **table, int size, char ***new_table);
void	free_calls(t_call **calls, int size);
int	check_calls(char **table);
int	is_invalid(char *str);
int	is_syntax(char *str);
int	has_unquoted_syntax(char *str);
int	get_calls(int n_elements, char **elements, t_call ***calls);
int	get_next_call(int *start, char **elements, t_call **call);
int	count_pipes(char **elements);
int len_portion(char **elements);
int	parse(char *input, t_call ***calls, t_params *params);

#endif
