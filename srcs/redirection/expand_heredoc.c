/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 15:53:18 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 15:53:36 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// find replacement
static char	*expand_env_in_heredoc(char *str, t_params *params,
			int size, char *buff)
{
	int		count;
	char	*tmp;

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
		if (ft_strncmp(params->env[count], str, size) == 0
			&& params->env[count][size] == '=')
			return (&params->env[count][size + 1]);
	}
	return (NULL);
}

// and replace
static char	*replace_var_heredoc(char *str, int first, char *line, int *i)
{
	char	*tmp;

	if (str == NULL)
	{
		ft_memmove(&line[first], &line[i[0]], ft_strlen(&line[i[0]]) + 1);
		i[0] = first - 1;
	}
	else
	{
		tmp = malloc(sizeof(char) * (first + ft_strlen(str)
					+ ft_strlen(&line[i[0]]) + 1));
		if (tmp == NULL)
			return (error_malloc_return("heredoc\n", NULL));
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

// expand in heredoc file
char	*expand_heredoc(char *line, t_params *params, int *i)
{
	int		count;
	int		first;
	char	*tmp;
	char	*tmp2;
	char	buff[12];

	count = 0;
	first = i[0];
	tmp = NULL;
	i[0]++;
	while (line[i[0]] != '$' && line[i[0]] != '\0' && line[i[0]] != '\''
		&& line[i[0]] != '\"' && (ft_space(line[i[0]]) != 0))
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
