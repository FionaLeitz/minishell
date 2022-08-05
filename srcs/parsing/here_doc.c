/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/05 17:25:48 by masamoil         ###   ########.fr       */
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
	close(fd_exist);
	return (pathname);
}

int	ft_here_doc(char *str)
{
	char	*delimiter;
	const char	*pathname;
	pid_t	child;
	int	fd;
	
	delimiter = ft_strtrim(str, " \t\n\v\f\r");
	pathname = hd_name();
	fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	if (fd == -1)
		perror("Error:");
//	signal(SIGINT, ft_sig_heredoc);
//	signal(SIGQUIT, ft_sig_heredoc);
	child = fork();
	if (child < 0)
		perror("Error:");
	if (child == 0)
	{
		get_hd_line(delimiter, fd);
		dup2(fd, STDIN_FILENO);
		close(fd);
		exit(0);
	}
	waitpid(child, NULL, 0);
	//close(fd);
	//unlink(pathname);
	return (0);
}

/*static int	print_error_heredoc(char *str)
{
	printf("warning:\n");
	printf("here-document delimited by end-of-file (wanted '%s')\n", str);
	return (-1);
}*/

void	get_hd_line(char *del, int fd)
{
	char	*line;
	char	*buff;
	char	*tmp;
	
	line = readline(">");
	buff = ft_strdup("\0");
	while (line)
	{
	//	if (!line)
	//		print_error_heredoc(del);
		if (ft_strcmp(line, del) == 0)
			break ;
		tmp = buff;
		buff = ft_strjoin(tmp, line);
		free(tmp);
		free(line);
		tmp = buff;
		buff = ft_strjoin(tmp, "\n");
		free(tmp);
		line = readline(">");
	}
	//write(fd, "\n", 1);
	write(fd, buff, ft_strlen(buff));
//	printf("\n");
//	printf("%s\n", buff);
}
