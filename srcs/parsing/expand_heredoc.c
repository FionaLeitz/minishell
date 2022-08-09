/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 11:42:13 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/09 16:42:33 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	jump_quotes_hd(char *str, int *i)
{
	char quote;

	quote = str[i[0]];
	i[0]++;
	while(str[i[0]] != '\0' && str[i[0]] != quote)
		i[0]++;
}

char *rep_hd(t_params *params, char *str, int size, int quote)
{
	int	count;
	if ((size == 0 && (ft_space(str[1]) == 0 || str[1] == '\0'))
		|| (quote % 2 != 0 && str[0] == '\"'))
		return ("$");
	count = -1;
	while(params->env[++count])
	{
		if (ft_strncmp(params->env[count], str, size) == 0 && params->env[count][size] == '=')
			return (&params->env[count][size + 1]);
	}
	return (NULL);
}

int	in_replace_hd(char *str, int s, char *line, int *i)
{
	char *tmp;

	if (str == NULL)
	{
		ft_memmove(&line[s], &line[i[0]], ft_strlen(&line[i[0]]) + 1);
		i[0] = s - 1;
	}
	else
	{
		tmp = malloc(sizeof(char) * (s + ft_strlen(str)
			+ ft_strlen(&line[i[0]]) + 1));
		if (tmp == NULL)
			return (-1);
		ft_bzero(tmp, s + ft_strlen(str) + ft_strlen(&line[i[0]]) + 1);
		tmp = ft_memmove(tmp, line, s);
		tmp = ft_strcat(tmp, str);
		tmp = ft_strcat(tmp, &line[i[0]]);
		line = strcpy(line, tmp);
		free(tmp);
		i[0] = s + ft_strlen(str) - 1;
	}
	return (0);
}

char	*expand_heredoc(char *line, t_params *params)
{
	int	i;
	int	s;
	int	quote;
	char	*new;	

	i = 0;
	quote = 0;
	while(line[i])
	{
		if (line[i] == '\"')
			quote++;
		if (line[i] == '\'' && quote % 2 == 0)
			jump_quotes_hd(line, &i);
		if (line[i] == '$')
		{
			s = i++;
			while(line[i] && ft_space(line[i]) != 0 && line[i] != '\''
				&& line[i] != '\"' && line[i] != '$')
				i++;
			ft_printf("%s\n", params->env[0]);
			new = rep_hd(params, &line[s + 1], i - s - 1, quote);
			//if (in_replace_hd(new, s, line, &i) == -1)
			//	return (-1);
		}
	}
	printf("line = %s\n", new);
	return (new);
}

