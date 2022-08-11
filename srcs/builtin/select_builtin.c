/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:11 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/10 09:30:12 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pipe(t_token *token, t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	int	i;

	i = -1;
	while (token)
	{
//		printf("-- token->fds[0] = %d\n-- token->fds[1] = %d\n", token->fds[0], token->fds[1]);
		pid[++i] = fork();
		if (pid[i] < 0)
			return (-1);
		if (pid[i] == 0)
		{
			if (i == 0)
				close(pipe_fd[i].raw[0]);
			else
			{
				dup2(pipe_fd[i - 1].raw[0], token->fds[0]);
				close(pipe_fd[i - 1].raw[0]);
			}
			if (token->next != NULL)
			{
				dup2(pipe_fd[i].raw[1], token->fds[1]);
				close(pipe_fd[i].raw[1]);
			}
			else
			{
				close(pipe_fd[i].raw[1]);
			}
			ft_select_builtin(token, params);
			close(pipe_fd[i].raw[0]);
			close(token->fds[0]);
			close(token->fds[1]);
			close(pipe_fd[i].raw[1]);
			exit(0);
		}
		close(pipe_fd[i].raw[1]);
		token = token->next;
	}
	close(pipe_fd[i].raw[0]);
	close(pipe_fd[i].raw[1]);
	while (i >= 0)
		waitpid(pid[i--], NULL, 0);
	return (0);
}

int	ft_execute(t_token *token, t_params *params)
{
	int			nbr;
	int			nbr2;
	t_token		*tmp;
	int			*pid;
	t_pipe_fd	*pipe_fd;

	nbr = 0;
	tmp = token;
	while (tmp)
	{
		nbr++;
		tmp = tmp->next;
	}
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
	}
	if (token->next == NULL && token->prev == NULL)
		ft_select_builtin(token, params);
	else
	{
		tmp = token;
		ft_pipe(tmp, params, pid, pipe_fd);
	}
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

// select if built-in or execve
void	ft_select_builtin(t_token *token, t_params *params)
{
	int	pid;

	if (ft_strncmp(token->args[0], "cd", 3) == 0)
		ft_cd(token->args, params);
	else if (ft_strncmp(token->args[0], "echo", 5) == 0)
		ft_echo(token->args);
	else if (ft_strncmp(token->args[0], "env", 4) == 0)
		ft_env(token->args, params);
	else if (ft_strncmp(token->args[0], "pwd", 4) == 0)
		ft_pwd(token->args);
	else if (ft_strncmp(token->args[0], "exit", 5) == 0)
		ft_exit(token->args);
	else if (ft_strncmp(token->args[0], "export", 7) == 0)
		ft_export(token->args, params);
	else if (ft_strncmp(token->args[0], "unset", 6) == 0)
		ft_unset(token->args, params);
	else
	{
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
		{
			if (access(token->args[0], F_OK | X_OK) == -1)
				get_path(token->args, params);
			execve(token->args[0], token->args, params->env);
			write(2, "minishell: ", 11);
			write(2, token->args[0], ft_strlen(token->args[0]));
			write(2, " : command not found\n", 21);
			exit(0);
		}
		waitpid(pid, NULL, 0);
	}
	return ;
}
