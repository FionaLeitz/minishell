/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/08 17:02:37 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

const char *hd_name(void)
{
	const char	*pathname;
	int	i;
	int	fd_exist;

	i = 1;
	fd_exist = 0;
	while (fd_exist != -1)
	{
		pathname = ft_strjoin("/tmp/hd", ft_itoa(i));
		fd_exist = open(pathname, O_RDONLY);
		if (fd_exist == -1)
			break;
		close(fd_exist);
		fd_exist = 0;
		++i;
	}
//	close(fd_exist);
	return (pathname);
}

int	ft_here_doc(char *str)
{
	char		*delimiter;
	const char	*pathname;
	pid_t		child;
	int			fd;

	delimiter = ft_strtrim(str, "\'");
	pathname = hd_name();
	fd = open(pathname, O_CREAT | O_WRONLY | O_TRUNC, 00664);
	if (fd == -1)
		perror("Error:");
	child = fork();
	if (child < 0)
		perror("Error:");
	if (child == 0)
	{
		dup2(STDOUT_FILENO, 1);
		ft_manage_sighd();
		get_hd_line(delimiter, fd);
		close(fd);
		dup2(STDOUT_FILENO, 1);
		dup2(STDIN_FILENO, 0);
		exit(0);
	}
	waitpid(child, NULL, 0);
	close(fd);
	unlink(pathname);
	return (0);
}

static int	print_error_heredoc(char *str)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document at line delimited by end-of-file", 2);
	ft_putstr_fd(" (wanted '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("')\n", 2);
//	close(fd);
	exit(0);
}

void	get_hd_line(char *del, int fd)
{
	char	*line;
	
	while (1)
	{
		line = readline("> ");	
		if (!line)
			print_error_heredoc(del);
//		line = expand_heredoc;
		if (ft_strcmp(line, del) == 0)
			break ;
		write(fd, line, ft_strlen(line) *  sizeof(char));
		write(fd, "\n", 1);
		free(line);
	}
}
