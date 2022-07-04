#include "../../minishell.h"


int	make_fd(int *pid, int **pipe_fd, int i)
{
	if (pipe_fd == NULL)
	{
		free(pid);
		return (1);
	}
	while (--i >= 0)
	{
		pipe_fd[i] = malloc(sizeof(int) * 2);
		if (pipe_fd[i] == NULL)
		{
			while (--i >= 0)
			{
				close(pipe_fd[i][0]);
				close(pipe_fd[i][1]);
				free(pipe_fd[i]);
			}
			free(pipe_fd);
			free(pid);
			return (1);
		}
		pipe(pipe_fd[i]);
	}
	return (0);
}

int	redirect(t_call *calls)
{
	int	fd;

	fd = 0;
	if (calls->funnels[0] != NULL)
	{
		ft_printf("Par ici !\n");
		fd = open(calls->funnels[0]->file_names[0], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("%c%s: %s\n", ft_tolower(strerror(errno)[0]),
				&strerror(errno)[1], calls->funnels[0]->file_names[0]);
			return (-1);
		}
	}
	return (fd);
}

int	children(t_call **calls, t_params *params, int *pid, int **pipe_fd)
{
	int	i;
	int	fd1;

	i = -1;
	while (calls[++i])
	{
		fd1 = redirect(calls[i]);
		ft_printf("fd1 = %d\n", fd1);
/*		if (calls[i]->funnels[0] == NULL)
			ft_printf("No funnels for command %d\n", i + 1);
		else
			ft_printf("Funnel exist for command %d\n\"%s\"  \"%s and %s\"\nnext funnel = %p\n", i + 1, calls[i]->funnels[0]->type_funnel, calls[i]->funnels[0]->file_names[0], calls[i]->funnels[0]->file_names[1], calls[i]->funnels[1]);
*/		pid[i] = fork();
		close(pipe_fd[i][1]);
		if (pid[i] == 0)
		{
			if (i == 0)
			{
				close(pipe_fd[i][0]);
			}
			if (i != 0)
			{
//				ft_printf("fd1 in child = %d\n", fd1);
				dup2(pipe_fd[i][1], fd1);
				dup2(pipe_fd[i][0], 0);
			}
			if (calls[i + 1])
			{
				if (fd1 == 1)
					fd1 = 0;
				dup2(pipe_fd[i + 1][1], 1);
				dup2(pipe_fd[i + 1][0], fd1);
			}
			params->env = ft_select_builtin(calls[i]->args, params);
//			close(fd1);
			return (1);
		}
	}
//	close(fd1);
	return (0);
}

int	free_pid_pipe(int i, int *pid, int **pipe_fd)
{
	free(pid);
	while (--i >= 0)
		free(pipe_fd[i]);
	free(pipe_fd);
	return (0);
}

int	command(t_call **calls, t_params *params)
{
	int	i;
	int	*pid;
	int	**pipe_fd;

	i = 0;
	while (calls[i])
		i++;
	pid = malloc(sizeof(int) * i);
	if (pid == NULL)
		return (1);
	pipe_fd = malloc(sizeof(int *) * i);
	if (make_fd(pid, pipe_fd, i) == 1)
		return (1);
	if (children(calls, params, pid, pipe_fd) == 1)
		return (free_pid_pipe(i, pid, pipe_fd));
	i = -1;
	while (calls[++i])
		waitpid(pid[i], NULL, 0);
	return (free_pid_pipe(i, pid, pipe_fd));
}
