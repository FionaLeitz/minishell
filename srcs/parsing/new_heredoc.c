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

void	free_in_heredoc(t_params *params, int fd)
{
	close(fd);
	dup2(params->old_fd[0], 0);
	close(params->old_fd[0]);
	free_struct(params->data);
	free_params(params);
}

// fct to get the line of the here_doc with readline as in prompt
int get_hd_line(char *del, int fd, int quotes, t_params *params)
{
	char *line;
	char *new;
	char	*tmp;
	
	new = NULL;

	while (1)
	{
		ft_signals(HEREDOC);
		line = readline("> ");
		if (!line && g_exit_st != 130)
		{
//			free_in_heredoc(params, fd);
			print_error_heredoc(del, fd, params);
//			break;
		}
		if (!line && g_exit_st == 130)
		{
			free_in_heredoc(params, fd);
			exit(130);
		}
		else
		{
			tmp = ft_strdup(line);
			if (tmp == NULL)
			{
				set_error_malloc("heredoc\n");
				return (-1);
			}
			if (line && ft_strcmp(line, del) == 0)
			{
				free(tmp);
				break;
			}
			if (line)
				new = write_hd(tmp, fd, quotes, params);
			if (new == NULL)
				return (-1);
		}
		replace_quotes(new);
		ft_putstr_fd(new, fd);
		ft_putstr_fd("\n", fd);
		free(new);
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
		if (line == NULL)
			return (NULL);
		i++;
	}
	new = ft_strdup(line);
	free(line);
	return(new);
}

char	*expand_env_in_heredoc(char *str, t_params *params, int size, char *buff)
{
	int count;
	char *tmp;


	tmp = NULL;
	if (size == 0)
 		return ("$");
	if (str[0] == '?' && (ft_space(str[0] == 0) || str[0] == '\0'))
	{
		ft_itoa_no_malloc(g_exit_st, buff);
		return (buff);
	}
	count = -1;
	while (params->env[++count])
	{
		if (ft_strncmp(params->env[count], str, size) == 0 && params->env[count][size] == '=')
			return (&params->env[count][size + 1]);
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
		{
			set_error_malloc("heredoc\n");
			return (NULL);
		}
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
	int		 count;
	int		first;
	char	*tmp;
	char	*tmp2;
	char	buff[12];

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
	tmp = expand_env_in_heredoc(&line[first + 1], params, count, buff);
	tmp2 = replace_var_heredoc(tmp, first, line, i);
	return (tmp2);
}

