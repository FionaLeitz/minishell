/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/04 17:32:36 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_here_doc(char *str)
{
	char	*delimiter;
	pid_t	child;
	int	pipefd[2];

		
	delimiter = ft_strtrim(str, " \t\n\v\f\r");
//	signal(SIGINT, ft_sig_heredoc);
//	signal(SIGQUIT, ft_sig_heredoc);
	if (pipe(pipefd) == -1)
		perror("Error:");
	child = fork();
	if (child < 0)
		perror("Error:");
	if (child == 0)
	{
		close(pipefd[0]);
		get_hd_line(delimiter, pipefd);
	}
	close(pipefd[1]);
	waitpid(child, NULL, 0);
	return (0);
}

/*static int	print_error_heredoc(char *str)
{
	printf("warning:\n");
	printf("here-document delimited by end-of-file (wanted '%s')\n", str);
	return (-1);
}*/

void	get_hd_line(char *del, int *pipefd)
{
	char	*line;
	char	*buff;
	char	*tmp;
	
	(void)pipefd;
	line = readline(">");
	buff = ft_strdup("\0");
	tmp = ft_strdup("\0");
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
	//write(pipefd[1], "\n", 1);
	//write(pipefd[1], buff, ft_strlen(buff));
	printf("\n");
	printf("%s\n", buff);
}
