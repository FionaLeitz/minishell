/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 11:42:13 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/29 20:22:43 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	jump_quotes_hd(char *str, int *i)
{
	char	quote;

	quote = str[i[0]];
	i[0]++;
	while (str[i[0]] != '\0' && str[i[0]] != quote)
		i[0]++;
}

char	*rep_hd(t_params *params, char *str, int size, int quote)
{
	int		count;
	char	*tmp;

	tmp = NULL;
	if ((size == 0 && (ft_space(str[1]) == 0 || str[1] == '\0'))
		|| (quote % 2 != 0 && str[0] == '\"'))
		return ("$");
	if (str[0] == '?' && (ft_space(str[1] == 0) || str[1] == '\0'))
		return (ft_itoa(g_exit_st));
	count = -1;
	while (params->env[++count])
	{
		if (ft_strncmp(params->env[count], str, size) == 0
			&& params->env[count][size] == '=')
			return (&params->env[count][size + 1]);
	}
	return (NULL);
}

char	*in_replace_hd(char *str, int s, char *line, int *i)
{
	char	*tmp;

	tmp = NULL;
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
			return (NULL);
		ft_bzero(tmp, s + ft_strlen(str) + ft_strlen(&line[i[0]]) + 1);
		tmp = ft_memmove(tmp, line, s);
		tmp = ft_strcat(tmp, str);
		tmp = ft_strcat(tmp, &line[i[0]]);
		line = ft_strdup(tmp);
		free(tmp);
		i[0] = s + ft_strlen(str) - 1;
	}
	return (line);
}

char	*expand_heredoc(char *line, t_params *params)
{
	int		i;
	int		s;
	int		quote;
	char	*new;
	char	*tmp;

	i = -1;
	quote = 0;
<<<<<<< HEAD
	new = NULL;
	while(line[i] != '\0')
=======
	while (line[++i] != '\0')
>>>>>>> dad9c212689baca2b14b5ad5f55c0601c20d519a
	{
		if (line[i] == '\"')
			quote++;
		if (line[i] == '\'' && quote % 2 == 0)
			jump_quotes_hd(line, &i);
		if (line[i] == '$')
		{
			printf("line in the beg = %s\n", line);
			s = i++;
<<<<<<< HEAD
			printf("s = %d\n", s);
			while(line[i] && ft_space(line[i]) != 0 && line[i] != '\''
				&& line[i] != '\"' && line[i] != '$')
				i++;
			tmp = rep_hd(params, &line[s + 1], i - s - 1, quote);
			printf("tmp = %s\n", tmp);
			//new = ft_strjoin(new,tmp);
=======
			while (line[i] && ft_space(line[i]) != 0 && line[i] != '\''
				&& line[i] != '\"' && line[i] != '$')
				i++;
			tmp = rep_hd(params, &line[s + 1], i - s - 1, quote);
>>>>>>> dad9c212689baca2b14b5ad5f55c0601c20d519a
			new = in_replace_hd(tmp, s, line, &i);
			printf("new = %s\n", new);
			printf("line = %s\n", line);
			printf("boucle est terminee\n");
			printf("-------------------------\n");
		}
	}
	return (new);
}
<<<<<<< HEAD

=======
>>>>>>> dad9c212689baca2b14b5ad5f55c0601c20d519a
