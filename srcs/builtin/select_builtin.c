/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:11 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/25 14:22:00 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// use dup2 to duplicate fds and read and write at the right place
static void	make_dup(t_token *token, t_pipe_fd *pipe_fd, int i)
{
	close((pipe_fd[i].raw[0]));
	if (i != 0 && token->fds[0] == 0)
	{
		dup2(pipe_fd[i - 1].raw[0], 0);
		close((pipe_fd[i - 1].raw[0]));
	}
	else if (token->fds[0] != 0)
	{
		dup2(token->fds[0], 0);
		close((token->fds[0]));
	}
	if (token->fds[1] != 1)
	{
		dup2(token->fds[1], 1);
		close((token->fds[1]));
	}
	else if (token->next != NULL)
	{
		dup2(pipe_fd[i].raw[1], 1);
		close((pipe_fd[i].raw[1]));
	}
}

// clean if execve is not executed
static void	clean_child(t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	t_token	*token;
	t_token	*tmp;

	token = params->data->head;
	free_params(params);
	free(pid);
	free(pipe_fd);
	free(params->data->trimmed);
	while (token != NULL)
	{
		tmp = token;
		free(tmp->value);
		free_table(tmp->args);
		free_table(tmp->red);
		token = token->next;
		free(tmp);
	}
	exit(0);
}

// what happens in child
static void	in_child(t_params *params, int *pid, t_pipe_fd *pipe_fd, int i)
{
	t_token	*tmp;
	t_token	*token;

	token = params->data->head;
	tmp = token;
	make_dup(token, pipe_fd, i);
	while (token)
	{
		close((pipe_fd[i].raw[0]));
		close((pipe_fd[i].raw[1]));
		i++;
		token = token->next;
	}
	ft_select_builtin(tmp, params, 1, NULL);
	clean_child(params, pid, pipe_fd);
}

// create childs
int	ft_pipe(t_token *token, t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	int		i;
	int		save;
	int		status;

	i = -1;
	while (token)
	{
		ft_signals(MUTE);
		pid[++i] = fork();
		check_child(pid[i]);
		if (pid[i] == 0)
			in_child(params, pid, pipe_fd, i);
		close((pipe_fd[i].raw[1]));
		if (i != 0)
			close((pipe_fd[i - 1].raw[0]));
		if (token->next == NULL)
			close((pipe_fd[i].raw[0]));
		token = token->next;
	}
	save = -1;
	while (++save <= i)
		waitpid(pid[save], &status, 0);
	g_exit_st = WEXITSTATUS(status);
	printf("exit status pipe = %d\n", g_exit_st);
	check_exit_status();
	return (0);
}

// execution if only one command
static int	only_one(t_token *token, t_params *params)
{
	int			old_fd[2];

	old_fd[0] = dup(0);
	old_fd[1] = dup(1);
	if (token->fds[0] != 0)
	{
		dup2(token->fds[0], 0);
		close(token->fds[0]);
	}
	if (token->fds[1] != 1)
	{
		dup2(token->fds[1], 1);
		close(token->fds[1]);
	}
	ft_select_builtin(token, params, 0, old_fd);
	dup2(old_fd[0], 0);
	dup2(old_fd[1], 1);
	close(old_fd[0]);
	close(old_fd[1]);
	return (0);
}

// create the pipes for every child
static int	create_pipe(t_pipe_fd *pipe_fd, int *pid, int nbr, int nbr2)
{
	if (pipe(pipe_fd[nbr].raw) == -1)
	{
		free(pid);
		while (--nbr2 > nbr)
		{
			close(pipe_fd[nbr2].raw[0]);
			close(pipe_fd[nbr2].raw[1]);
		}
		free(pipe_fd);
		return (-1);
	}
	return (0);
}

// start the execution by making pipes and childs
int	ft_execute(t_token *token, t_params *params)
{
	int			nbr;
	int			nbr2;
	int			*pid;
	t_pipe_fd	*pipe_fd;

	if (token->next == NULL && token->prev == NULL)
		return (only_one(token, params));
	nbr = ft_size(token);
	pid = malloc(sizeof(int) * nbr);
	if (pid == NULL)
		return (-1);
	pipe_fd = malloc(sizeof(t_pipe_fd) * nbr);
	if (pipe_fd == NULL)
	{
		free(pid);
		return (-1);
	}
	nbr2 = nbr;
	while (--nbr >= 0)
		if (create_pipe(pipe_fd, pid, nbr, nbr2) == -1)
			return (-1);
	ft_pipe(token, params, pid, pipe_fd);
	free(pid);
	free(pipe_fd);
	return (0);
}

// create possible path
static int	error_path(char **path, int	*i, char *arg)
{
	char	*tmp;

	tmp = ft_strcat_malloc(path[i[0]], "/");
	if (tmp == NULL)
	{
		free_table(path);
		return (-1);
	}
	free(path[i[0]]);
	path[i[0]] = ft_strcat_malloc(tmp, arg);
	free(tmp);
	if (path[i[0]] == NULL)
	{
		free_table(path);
		free_table(&path[i[0] + 1]);
		return (-1);
	}
	return (0);
}

// get path of command if not built-in
int	get_path(char **arg, t_params *params)
{
	int		i;
	char	**path;

	i = 0;
	while (params->env[i] && ft_strncmp(params->env[i], "PATH=", 5) != 0)
		i++;
	if (params->env[i] == NULL)
		return (-1);
	path = ft_split(&params->env[i][5], ':');
	if (path == NULL)
		return (-1);
	i = -1;
	while (path[++i])
	{
		if (error_path(path, &i, arg[0]) == -1)
			return (-1);
		if (access(path[i], F_OK | X_OK) != -1)
		{
			free(arg[0]);
			arg[0] = ft_strdup(path[i]);
			break ;
		}
	}
	free_table(path);
	return (0);
}

// if command not found
static void	command_no(t_token *token, t_params *params, int *old_fd)
{
	write(2, "minishell: ", 11);
	write(2, token->args[0], ft_strlen(token->args[0]));
	write(2, " : command not found\n", 21);
	g_exit_st = 127;
	free_params(params);
	free(params->data->trimmed);
	free(token->value);
	free_table(token->args);
	free_table(token->red);
	free(token);
	if (old_fd != NULL)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	exit(0);
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
		if (pid < 0)
			return ;
		//check_child(pid);
	}
	if (pid == 0)
	{
		ft_signals(COMMAND);
		if (access(token->args[0], F_OK | X_OK) == -1)
			get_path(token->args, params);
		execve(token->args[0], token->args, params->env);
		command_no(token, params, old_fd);
	}
	if (i == 0)
	{
		waitpid(pid, &status, 0);
		g_exit_st = WEXITSTATUS(status);
	//	if (pid != -1 && (0 < waitpid(pid, &g_exit_st, 0)))
          //     		g_exit_st = WEXITSTATUS(g_exit_st);
		check_exit_status();
		//printf("exit_status command = %d\n", g_exit_st);
	}
	ft_signals(DEFAULT);
}

// select if built-in
void	ft_select_builtin(t_token *token, t_params *params, int i, int *old_fd)
{
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
		g_exit_st = ft_exit(token->args);
	else if (ft_strncmp(token->args[0], "export", 7) == 0)
		g_exit_st = ft_export(token->args, params);
	else if (ft_strncmp(token->args[0], "unset", 6) == 0)
		g_exit_st = ft_unset(token->args, params);
	else
		make_command(token, params, i, old_fd);
	return ;
}
