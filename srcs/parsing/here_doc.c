/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 15:44:06 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/18 16:42:57 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//used to create a new here_doc name each time hd1, hd2, etc...
const char *hd_name(void)
{
	const char	*pathname;
	int	i;
	int	fd_exist;
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
			break;
		close(fd_exist);
		fd_exist = 0;
		++i;
	}
//	close(fd_exist);
	return (pathname);
}

//main fct of here_doc, check delimiter, eventually change the return to return fd
int	ft_here_doc(char *delim, t_params *params)
{
	const char	*pathname;
	int		fd;
	int		quotes;
	char		*delim_tmp;

	delim_tmp = delim;
	quotes = check_delim(delim);
	if (delim_quotes(delim) == 1)
		delim = del_quotes_hd(delim);
	else
		delim = delim_tmp; 
	pathname = hd_name();
	fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	get_hd_line(delim, fd, quotes, params);
	/*{
		close(fd);
		//dup2(0, STDIN_FILENO);
		return (-1);
	}*/
	ft_signals(DEFAULT);
	close(fd);
	fd = get_fd_input((char*)pathname, "<");
	unlink(pathname);
	free((char*)pathname);
	return (fd);
}


//fct to get the line of the here_doc with readline as in prompt
void	get_hd_line(char *del, int fd, int quotes, t_params *params)
{
	char	*line;
	char	*new;
	
	line = NULL;
	new = NULL;
	while (1)
	{
		line = readline("> ");	
		if (!line)
		{
			print_error_heredoc(del, fd);
			break ;
		}
		if (line && ft_strcmp(line, del) == 0)
			break ;
		if (line && ft_if_char(line, '$') == 0 && quotes == 0)
			new = write_hd_expand(line, fd, params);
		else if(line)
			new = ft_strdup(line);
		ft_putstr_fd(new, fd);
		ft_putstr_fd("\n", fd);
		free(new);
	}
	//-----------------------------version2----------------------------------
	//line = ft_strdup("\0");
	//line = readline("> ");
	//if (!line)
	//	print_error_heredoc(del, fd);
	// while (line)
	// {
	// 	ft_signals(HEREDOC);
	// 	if (!line && exit_st != 130)
	// 	{
	// 		dprintf(1, "exit status = %d\n", exit_st);
	// 		print_error_heredoc(del, fd);
	// 		break ;
	// 	}
	// 	//if (exit_st == 130)
	// 	//	break ;
	// 	if (line && ft_strcmp(line, del) == 0)
	// 		break ;
	// 	if (line && ft_if_char(line, '$') == 0 && quotes == 0)
	// 		new = write_hd_expand(line, fd, params);
	// 	else if(line)
	// 		new = ft_strdup(line);
	// 	ft_putstr_fd(new, fd);
	// 	ft_putstr_fd("\n", fd);
	// 	free(new);
	// }
	//---------------------------------version3----------------------------
	// while (line)
	// {
	// 	ft_signals(HEREDOC);
	// 	free(line);
	// 	line = NULL;
	// 	line = readline("> ");	
	// 	if (!line && exit_st != 130)
	// 		print_error_heredoc(del, fd);
	// 	if (!line && exit_st == 130)
	// 		return (-1);
	// 	else
	// 	{
	// 		if (line && ft_strcmp(line, del) == 0)
	// 			break ;
	// 		if (line && ft_if_char(line, '$') == 0 && quotes == 0)
	// 			new = write_hd_expand(line, fd, params);
	// 		else if(line)
	// 			new = ft_strdup(line);
	// 		ft_putstr_fd(new, fd);
	// 		ft_putstr_fd("\n", fd);
	// 		free(new);
	// 	}
	// }
	// return (0);
}

char	*write_hd_expand(char *line, int fd, t_params *params)
{
	int	i;
	int	save;
	char	*tmp;
	char 	*new;
	char	*buff;
	
	(void)fd;
	i = 0;
	save = 0;
	tmp = NULL;
	new = NULL;
	buff = NULL;
	while(line[i] != '\0')
	{
		if(line[i] == '$')
		{	
			save = i;
			//printf("save = %d, i = %d \n", save, i);
			tmp = expand_heredoc(&line[i], params);
		//	printf("tmp $ = %s\n", tmp);
			if (tmp == NULL)
				new = &line[save - i];
			else
			{	
				buff = ft_strndup(&line[save - i], save);
				//printf("buff = %s\n", buff);
				new = ft_strjoin(buff, tmp);
				//printf("new = %s\n", new);
			}
		}
		i++;
	}
	return (new);
}
