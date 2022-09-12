/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 09:58:48 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/02 10:56:38 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// end while in get_hd_line function
static void	end_new(char *new, int fd)
{
	replace_quotes(new);
	ft_putstr_fd(new, fd);
	ft_putstr_fd("\n", fd);
	free(new);
}

// check signals in get_hd_line
static void	check_signals(char *line, char *del, int fd, t_params *params)
{
	if (!line && g_exit_st != 130)
		print_error_heredoc(del, fd, params);
	if (!line && g_exit_st == 130)
	{
		free_in_heredoc(params, fd);
		exit(130);
	}
}

// verify if expand needed
static char	*line_to_write(char *line, int quotes, t_params *params)
{
	int		i;
	char	*new;

	i = 0;
	new = NULL;
	while (line[i])
	{
		if (line[i] == '$' && quotes == 0)
			line = expand_heredoc(line, params, &i);
		if (line == NULL)
			return (NULL);
		i++;
	}
	new = ft_strdup(line);
	free(line);
	return (new);
}

// fct to get the line of the here_doc with readline as in prompt
int	get_hd_line(char *del, int fd, int quotes, t_params *params)
{
	char	*line;
	char	*new;
	char	*tmp;

	new = NULL;
	while (1)
	{
		ft_signals(HEREDOC);
		line = readline("> ");
		check_signals(line, del, fd, params);
		tmp = ft_strdup(line);
		if (tmp == NULL)
			return (set_error_malloc("heredoc\n"));
		if (line && ft_strcmp(line, del) == 0)
		{
			free(tmp);
			break ;
		}
		if (line)
			new = line_to_write(tmp, quotes, params);
		if (new == NULL)
			return (-1);
		end_new(new, fd);
	}
	return (0);
}
