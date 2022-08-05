/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/05 15:22:09 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

const char *hd_name(void)
{
	const char	*pathname;
	int	i;
	int	fd_exist;
	char	*no;

	i = 1;
	pathname = ft_strdup("/tmp/hd");
	fd_exist = open(pathname, O_RDONLY);
	while (fd_exist != -1)
	{
		close(fd_exist);
		no = ft_itoa(i);
		pathname = ft_strjoin(pathname, no);
		fd_exist = open(pathname, O_RDONLY); 
		i++;
	}
	if (fd_exist != -1)
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
//	signal(SIGINT, ft_sig_heredoc);
//	signal(SIGQUIT, ft_sig_heredoc);
	child = fork();
	if (child < 0)
		perror("Error:");
	if (child == 0)
	{
		get_hd_line(delimiter, fd);
	}
	waitpid(child, NULL, 0);
	close(fd);
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
	write(fd, "\n", 1);
	write(fd, buff, ft_strlen(buff));
	//printf("\n");
	//printf("%s\n", buff);
}
