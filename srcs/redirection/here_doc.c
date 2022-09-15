/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/30 12:05:47 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//used to create a new here_doc name each time hd1, hd2, etc...
static char	*hd_name(void)
{
	char	*pathname;
	int		i;
	int		fd_exist;
	char	*tmp;

	i = 1;
	fd_exist = 0;
	while (1)
	{
		tmp = ft_itoa(i);
		if (tmp == NULL)
			return (error_malloc_return("heredoc\n", NULL));
		pathname = ft_strjoin("/tmp/hd", tmp);
		free(tmp);
		if (pathname == NULL)
			return (error_malloc_return("heredoc\n", NULL));
		fd_exist = open(pathname, O_RDONLY);
		if (fd_exist == -1)
			break ;
		close(fd_exist);
		++i;
		free(pathname);
	}
	return (pathname);
}

// in child
static int	then_child(char *delim, int *utils, t_params *params, char *path)
{
	free(path);
	get_hd_line(delim, utils[0], utils[1], params);
	if (utils[0] != -1)
		close(utils[0]);
	free_struct(params->data);
	free_params(params);
	close(params->old_fd[0]);
	if (errno == 12)
		exit(12);
	exit(g_exit_st);
}

// create child for heredoc
static int	fork_heredoc(char *delim, int *utils, t_params *params, char *path)
{
	pid_t	pid;

	pid = fork();
	if (check_child(pid) == -1)
	{
		unlink(path);
		free(path);
		return (-1);
	}
	if (pid == 0)
		return (then_child(delim, utils, params, path));
	if (pid != -1 && (0 < waitpid(pid, &g_exit_st, 0)))
		g_exit_st = WEXITSTATUS(g_exit_st);
	if (WIFSIGNALED(g_exit_st) && WTERMSIG(g_exit_st))
	{
		unlink(path);
		free(path);
		close(utils[0]);
		g_exit_st = 130;
		return (1);
	}
	return (0);
}

// check error in ft_here_doc
static int	verify_error(char *pathname, t_params *params, int child)
{
	if (errno == 12)
	{
		unlink(pathname);
		close(STDIN_FILENO);
		unlink(pathname);
		free(pathname);
		exit(free_exit(params, params->data, NULL));
	}
	if (check_child(child) == -1)
	{
		unlink(pathname);
		free(pathname);
		return (-1);
	}
	return (0);
}

//main fct of here_doc, check delimiter, creates heredoc in child(fork) 
int	ft_here_doc(char *delim, t_params *params)
{
	char		*pathname;
	int			utils[2];
	int			child;

	utils[1] = delim_quotes(delim);
	pathname = hd_name();
	if (pathname == NULL)
	{
		close(STDIN_FILENO);
		exit(free_exit(params, params->data, NULL));
	}
	utils[0] = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	ft_signals(MUTE);
	child = fork_heredoc(delim, utils, params, pathname);
	if (child == 1)
		return (-2);
	if (verify_error(pathname, params, child) == -1)
		return (-1);
	ft_signals(DEFAULT);
	if (utils[0] != -1)
		close(utils[0]);
	utils[0] = get_fd_input(pathname, "<");
	unlink(pathname);
	free(pathname);
	return (utils[0]);
}
