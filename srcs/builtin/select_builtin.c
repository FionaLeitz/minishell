/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:11 by fleitz            #+#    #+#             */
/*   Updated: 2022/09/01 10:13:03 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// create possible path
static int	error_path(char **path, int	*i, char *arg)
{
	char	*tmp;

	(void)arg;
	tmp = ft_strcat_malloc(path[i[0]], "/");
	if (tmp == NULL)
	{
		free_table(path);
		return (set_error_malloc("execution\n"));
	}
	free(path[i[0]]);
	path[i[0]] = ft_strcat_malloc(tmp, arg);
	free(tmp);
	if (path[i[0]] == NULL)
	{
		while (path[++i[0]])
			free(path[i[0]]);
		free_table(path);
		return (set_error_malloc("execution\n"));
	}
	return (0);
}

// free all if error with command
static void	free_command_no(t_params *params, int *old_fd)
{
	t_token	*tmp;

	tmp = params->data->head;
	while (tmp)
	{
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		tmp = tmp->next;
	}
	free(params->data->pid);
	free(params->data->pipe_fd);
	free_struct(params->data);
	free_params(params);
	if (old_fd != NULL)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	exit(g_exit_st);
}

// check if command is a directory
static int	path_directory(char	**arg, t_token *tok, t_params *par, int *fd)
{
	struct stat	*test;

	test = malloc(sizeof(struct stat));
	if (test == NULL)
		return (set_error_malloc("execution\n"));
	if (stat(arg[0], test) >= 0 && S_ISDIR(test->st_mode) == 1)
	{
		free(test);
		write(2, "minishell: ", 11);
		write(2, tok->args[0], ft_strlen(tok->args[0]));
		write(2, ": Is a directory\n", 18);
		g_exit_st = 126;
		free_command_no(par, fd);
	}
	free(test);
	return (0);
}

// find potential command
static int	find_path(char **arg, t_params *params, int i)
{
	char	**path;

	path = ft_split(&params->env[i][5], ':');
	if (path == NULL)
		return (set_error_malloc("execution\n"));
	i = -1;
	while (path[++i])
	{
		if (error_path(path, &i, arg[0]) == -1)
			return (-1);
		if (access(path[i], F_OK | X_OK) != -1)
		{
			free(arg[0]);
			arg[0] = ft_strdup(path[i]);
			if (arg[0] == NULL)
				set_error_malloc("execution\n");
			break ;
		}
	}
	free_table(path);
	return (0);
}

// check if command can be found
static int	get_path(char **arg, t_token *token, t_params *params, int *old_fd)
{
	int	i;

	i = 0;
	if (arg[0][0] == '\0')
		return (0);
	if (path_directory(arg, token, params, old_fd) == -1)
		return (-1);
	if (access(arg[0], F_OK | X_OK) != -1)
		return (0);
	while (params->env[i] && ft_strncmp(params->env[i], "PATH=", 5) != 0)
		i++;
	if (params->env[i] == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_st = 127;
		free_command_no(params, old_fd);
	}
	if (find_path(arg, params, i) == -1)
		return (-1);
	return (0);
}

// execute command and write error if does not work
static int	execute_command(t_token *token, t_params *params, int *old_fd)
{
	ft_signals(COMMAND);
	get_path(token->args, token, params, old_fd);
	if (errno == 12)
		return (-1);
	execve(token->args[0], token->args, params->env);
	write(2, "minishell: ", 11);
	write(2, token->args[0], ft_strlen(token->args[0]));
	write(2, ": command not found\n", 20);
	g_exit_st = 127;
	free_command_no(params, old_fd);
	return (0);
}

// execve if not built-in
static void	make_command(t_token *token, t_params *params, int i, int *old_fd)
{
	int	pid;
	int	status;

	pid = 0;
	status = 0;
	if (i == 0)
	{
		ft_signals(MUTE);
		pid = fork();
		if (check_child(pid) == -1)
			return ;
	}
	if (pid == 0)
		if (execute_command(token, params, old_fd) == -1)
			return ;
	if (i == 0)
	{
		if (0 < waitpid(pid, &g_exit_st, 0) && (WIFEXITED(g_exit_st)))
			g_exit_st = WEXITSTATUS(g_exit_st);
		else if (WIFSIGNALED(g_exit_st))
			g_exit_st = 128 + WTERMSIG(g_exit_st);
		check_exit_status();
	}
	ft_signals(DEFAULT);
}

// select if built-in
void	ft_select_builtin(t_token *token, t_params *params, int i, int *old_fd)
{
	if (token->fds[0] == -1 || token->fds[1] == -1)
		return ;
	if (token->args[0] == NULL)
		return ;
	if (ft_strncmp(token->args[0], "cd", 3) == 0)
		g_exit_st = ft_cd(token->args, params);
	else if (ft_strncmp(token->args[0], "echo", 5) == 0)
		g_exit_st = ft_echo(token->args);
	else if (ft_strncmp(token->args[0], "env", 4) == 0)
		g_exit_st = ft_env(token->args, params);
	else if (ft_strncmp(token->args[0], "pwd", 4) == 0)
		g_exit_st = ft_pwd(token->args);
	else if (ft_strncmp(token->args[0], "exit", 5) == 0)
		g_exit_st = ft_exit(token->args, i, params, old_fd);
	else if (ft_strncmp(token->args[0], "export", 7) == 0)
		g_exit_st = ft_export(token->args, params);
	else if (ft_strncmp(token->args[0], "unset", 6) == 0)
		g_exit_st = ft_unset(token->args, params);
	else
		make_command(token, params, i, old_fd);
	return ;
}
