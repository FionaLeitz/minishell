/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:11 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/30 10:32:59 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
static int	get_path(char **arg, t_params *params)
{
	int		i;
	char	**path;
	struct stat *test;

	i = 0;
	if (arg[0][0] == '\0')
		return (0);
	test = malloc(sizeof(struct stat));
	if (stat(arg[0], test) >= 0 && S_ISDIR(test->st_mode) == 1)
	{
		free(test);
		return (126);
	}
	else if (access(arg[0], F_OK | X_OK) != -1)
	{
		free(test);
		return (0);
	}
	free(test);
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
static void	command_no(t_token *token, t_params *params, int *old_fd, int i)
{
	if (i == 1)
	{
		write(2, "minishell: ", 11);
		write(2, token->args[0], ft_strlen(token->args[0]));
		write(2, ": Is a directory\n", 18);
		g_exit_st = 126;
	}
	else
	{
		write(2, "minishell: ", 11);
		write(2, token->args[0], ft_strlen(token->args[0]));
		write(2, ": command not found\n", 20);
		g_exit_st = 127;
	}
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
	exit(g_exit_st);
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
	}
	if (pid == 0)
	{
		ft_signals(COMMAND);
		if (get_path(token->args, params) == 126)
			command_no(token, params, old_fd, 1);
		execve(token->args[0], token->args, params->env);
		command_no(token, params, old_fd, 0);
	}
	if (i == 0)
	{
		if (0 < waitpid(pid, &g_exit_st, 0) && (WIFEXITED(g_exit_st)))
			g_exit_st = WEXITSTATUS(g_exit_st);
		else if (WIFSIGNALED(g_exit_st))
			g_exit_st = 128 + WTERMSIG(g_exit_st);
		else if (WIFSTOPPED(g_exit_st))
			g_exit_st = 128 + WSTOPSIG(g_exit_st);
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
		g_exit_st = ft_exit(token->args, i);
	else if (ft_strncmp(token->args[0], "export", 7) == 0)
		g_exit_st = ft_export(token->args, params);
	else if (ft_strncmp(token->args[0], "unset", 6) == 0)
		g_exit_st = ft_unset(token->args, params);
	else
		make_command(token, params, i, old_fd);
	return ;
}
