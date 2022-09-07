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
char	*hd_name(void)
{
	char	*pathname;
	int		i;
	int		fd_exist;
	char	*tmp;

	i = 1;
	fd_exist = 0;
	while (fd_exist != -1)
	{
		tmp = ft_itoa(i);
		if (tmp == NULL)
		{
			set_error_malloc("heredoc\n");
			return (NULL);
		}
		pathname = ft_strjoin("/tmp/hd", tmp);
		free(tmp);
		if (pathname == NULL)
		{
			set_error_malloc("heredoc\n");
			return (NULL);
		}
		fd_exist = open(pathname, O_RDONLY);
		if (fd_exist == -1)
			break ;
		close(fd_exist);
		fd_exist = 0;
		++i;
	}
	return (pathname);
}

static int	fork_heredoc(char *delim, int *utils, t_params *params, char *path)
{
	pid_t	pid;

	pid = fork();
	if (check_child(pid) == -1)
		return (-1);
	if (pid == 0)
	{
		if (get_hd_line(delim, utils[0], utils[1], params) == 1)
			return (-1);
		close(utils[0]);
		free_struct(params->data);
		free_params(params);
		free(path);
		if (errno == 12)
			exit(12);
		exit(g_exit_st);
	}
	if (pid != -1 && (0 < waitpid(pid, &g_exit_st, 0)))
		g_exit_st = WEXITSTATUS(g_exit_st);
	if (WIFSIGNALED(g_exit_st) && WTERMSIG(g_exit_st))
	{
		g_exit_st = 130;
		return (1);
	}
	return (0);
}

//main fct of here_doc, check delimiter, creates heredoc in child(fork) 
int	ft_here_doc(char *delim, t_params *params)
{
	char		*pathname;
	int			utils[2];
	char		*delim_tmp;
	int			child;


	g_exit_st = 0;
	delim_tmp = delim;
	utils[1] = check_delim(delim);
	if (delim_quotes(delim) == 1)
		delim = del_quotes_hd(delim);
	else
		delim = delim_tmp;
	pathname = hd_name();
	if (pathname == NULL)
	{
		close(STDIN_FILENO);
		free_exit(params, params->data, NULL);
		exit (12);
	}
	utils[0] = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	ft_signals(MUTE);
	child = fork_heredoc(delim, utils, params, pathname);
	if (errno == 12)
	{
		close(STDIN_FILENO);
		free(pathname);
		free_exit(params, params->data, NULL);
		exit (12);
	}
	check_child(child);
	ft_signals(DEFAULT);
	close(utils[0]);
	utils[0] = get_fd_input(pathname, "<");
	unlink(pathname);
	free(pathname);
	return (utils[0]);
}
