/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 09:58:48 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/31 11:17:53 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

// fct to get the line of the here_doc with readline as in prompt
int get_hd_line(char *del, int fd, int quotes, t_params *params)
{
	char *line;
	char *new;
	char	*tmp;
	
	line = ft_strdup("\0");
	new = NULL;
	while (line)
	{
		ft_signals(HEREDOC);
		line = readline("> ");
		if (!line && g_exit_st != 130)
		{
			print_error_heredoc(del, fd);
			break;
		}
		if (!line && g_exit_st == 130)
			return (-1);
		else
		{
			tmp = ft_strdup(line);
			if (line && ft_strcmp(line, del) == 0)
				break;
			if (line)
				new = write_hd(tmp, fd, quotes, params);
		}
		ft_putstr_fd(new, fd);
		ft_putstr_fd("\n", fd);
//		free(line);
//		free(tmp);
	}
	return (0);
}

char	*write_hd(char *line, int fd, int quotes, t_params *params)
{
	int	i;
	char *new;

	(void)fd;
	i = 0;
	new = NULL;

	while (line[i])
	{
		if (line[i] == '$' && quotes == 0)
			line = expand_heredoc(line, params, &i);
		i++;
	}
	return(line);
}

char	*expand_env_in_heredoc(char *str, t_params *params, int size)
{
	int count;
	char *tmp;
	char	buff[12];

	tmp = NULL;
	if (size == 0)
 		return (ft_strdup("$"));
	if (str[0] == '?' && (ft_space(str[0] == 0) || str[0] == '\0'))
	{
		ft_itoa_no_malloc(g_exit_st, buff);
		return (ft_strdup(buff));
	}
	count = -1;
	while (params->env[++count])
	{
		if (ft_strncmp(params->env[count], str, size) == 0 && params->env[count][size] == '=')
			return (ft_strdup(&params->env[count][size + 1]));
	}
	return (NULL);
}

char	*replace_var_heredoc(char *str, int first, char *line, int *i)
{
	char	*tmp;

	if (str == NULL)
	{
		ft_memmove(&line[first], &line[i[0]], ft_strlen(&line[i[0]]) + 1);
		i[0] = first - 1;
	}
	else
	{
		tmp = malloc(sizeof(char) * (first + ft_strlen(str) + ft_strlen(&line[i[0]]) + 1));
		if (tmp == NULL)
			return (NULL);
		ft_bzero(tmp, first + ft_strlen(str)
			+ ft_strlen(&line[i[0]]) + 1);
		tmp = ft_memmove(tmp, line, first);
		tmp = ft_strcat(tmp, str);
		tmp = ft_strcat(tmp, &line[i[0]]);
		free(line);
		line = tmp;
		i[0] = first + ft_strlen(str) - 1;
	}
	return (line);
}

char	*expand_heredoc(char *line, t_params *params, int *i)
{
	int	 count;
	int		first;
	char *tmp;

	count = 0;
	first = i[0];
	tmp = NULL;
	i[0]++;
	while (line[i[0]] != '$' && line[i[0]] != '\0' && line[i[0]] != '\'' && line[i[0]] != '\"'
		&& (ft_space(line[i[0]]) != 0))
	{
		count++;
		i[0]++;
		if (line[i[0] - 1] == '?')
			break ;
	}
	tmp = expand_env_in_heredoc(&line[first + 1], params, count);
	line = replace_var_heredoc(tmp, first, line, i);
	return(line);
}
