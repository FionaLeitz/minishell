/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/03 18:25:22 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*here_doc(char *str)
{	
	char	*delimiter;

	delimiter = ft_strtrim(str, " \t\n\v\f\r");
	get_hd_line(delimiter);
	return (delimiter);
}

/*static int	print_error_heredoc(char *str)
{
	printf("warning:\n");
	printf("here-document delimited by end-of-file (wanted '%s')\n", str);
	return (-1);
}*/

void	get_hd_line(char *del)
{
	char	*line;
	char	*buff;
	char	*tmp;

	line = readline(">");
	buff = ft_strdup("\0");
	tmp = ft_strdup("\0");
	while (line)
	{
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
	printf("\n");
	printf("%s\n", buff);
}
