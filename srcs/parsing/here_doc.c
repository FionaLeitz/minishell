/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/25 14:22:20 by masamoil         ###   ########.fr       */
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
		pathname = ft_strjoin("/tmp/hd", tmp);
		free(tmp);
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
	check_child(pid);
	if (pid == 0)
	{
		get_hd_line(delim, utils[0], utils[1], params);
		close(utils[0]);
		free_struct(params->data);
		free_params(params);
		free(path);
		exit(g_exit_st);
	}
	if (pid != -1 && (0 < waitpid(pid, &g_exit_st, 0)))
		g_exit_st = WEXITSTATUS(g_exit_st);
	if (WIFSIGNALED(g_exit_st) && WTERMSIG(g_exit_st) == 2)
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
	utils[0] = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	ft_signals(MUTE);
	child = fork_heredoc(delim, utils, params, pathname);
	ft_signals(DEFAULT);
	close(utils[0]);
	utils[0] = get_fd_input(pathname, "<");
	unlink(pathname);
	free(pathname);
	return (utils[0]);
}

//fct to get the line of the here_doc with readline as in prompt
int	get_hd_line(char *del, int fd, int quotes, t_params *params)
{
	char	*line;
	char	*new;

	line = NULL;
	new = NULL;
	ft_signals(HEREDOC);
	dprintf(1, "exit status in child = %d\n", g_exit_st);
	while (1)
	{
		line = readline("> ");
		if (!line && g_exit_st != 130)
		{
			print_error_heredoc(del, fd);
			break ;
		}
		if (!line && g_exit_st == 130)
			return (2);
		if (line && ft_strcmp(line, del) == 0)
			break ;
		if (ft_if_char(line, '$') == 0 && quotes == 0)
			new = write_hd_expand(line, fd, params);
		else if (line)
			new = ft_strdup(line);
		ft_putstr_fd(new, fd);
		ft_putstr_fd("\n", fd);
		free(new);
	}
	return (0);
}

char	*write_hd_expand(char *line, int fd, t_params *params)
{
	int		i;
	int		save;
	char	*tmp;
	char	*new;
	char	*buff;

	(void)fd;
	i = 0;
	save = 0;
	tmp = NULL;
	new = NULL;
	buff = NULL;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{	
			save = i;
			tmp = expand_heredoc(&line[i], params);
			if (tmp == NULL)
				new = &line[save - i];
			else
			{	
				buff = ft_strndup(&line[save - i], save);
				new = ft_strjoin(buff, tmp);
			}
		}
		i++;
	}
	return (new);
}
