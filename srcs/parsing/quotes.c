/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:39:29 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/12 12:11:25 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if next quote exist
int	get_next_quote(char quote, t_data *data)
{
	while (data->trimmed[data->i] && data->trimmed[data->i] != quote)
		data->i++;
	if (data->trimmed[data->i] == quote)
		return (0);
	return (-1);
}

// check if quotes opened are closed correctly
int	check_quotes(t_data *data)
{
	char	quote;

	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quote = data->trimmed[data->i];
			data->i++;
			if (get_next_quote(quote, data) == -1)
				return (-1);
		}
		data->i++;
	}
	return (0);
}

// suppress quotes
int	in_del_quote(char *str, int j)
{
	char	quote;

	quote = str[j];
	ft_memcpy(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
	while (str[j] && str[j] != quote)
		j++;
	ft_memcpy(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
	return (j);
}

static int	in_del_quotes_redir(char *str)
{
	int		j;
	char	*tmp;

	j = 0;
	while (str[j] == '>' || str[j] == '<')
		j++;
	if (ft_space(str[j]) == 0 || ft_space(str[ft_strlen(str) - 1]) == 0)
	{
		tmp = malloc(sizeof(char) * ft_strlen(str) + 3);
		if (tmp == NULL)
			return (set_error_malloc("parse\n"));
		ft_bzero(tmp, ft_strlen(str) + 3);
		tmp[0] = str[0];
		tmp[1] = str[1];
		tmp[j] = '\'';
		ft_strcat(tmp, &str[j]);
		tmp[ft_strlen(tmp)] = '\'';
		free(str);
		str = tmp;
	}
	return (0);
}

// find suppressable quotes in redirections
int	del_quotes_redir(t_token *token)
{
	int		i;
	int		j;

	while (token)
	{
		i = -1;
		while (token->red[++i])
		{
			j = -1;
			while (token->red[i][++j] != '\0')
			{
				if (token->red[i][0] == '<' && token->red[i][1] == '<')
					break ;
				if (token->red[i][j] == '\'' || token->red[i][j] == '\"')
					j = in_del_quote(token->red[i], j);
				if (token->red[i][j] == '\0')
					break ;
				if (token->red[i][j] == '\'' || token->red[i][j] == '\"')
					j--;
			}
			if (in_del_quotes_redir(token->red[i]) == -1)
				return (-1);
		}
		token = token->next;
	}
	return (0);
}

// find suppressable quotes
int	del_quotes(t_token *token)
{
	t_token	*tmp;
	int		i;
	int		j;

	tmp = token;
	while (tmp)
	{
		i = -1;
		while (tmp->args[++i])
		{
			j = -1;
			while (tmp->args[i][++j] != '\0')
			{
				if (tmp->args[i][j] == '\'' || tmp->args[i][j] == '\"')
					j = in_del_quote(tmp->args[i], j);
				if (tmp->args[i][j] == '\0')
					break ;
				if (tmp->args[i][j] == '\'' || tmp->args[i][j] == '\"')
					j--;
			}
		}
		tmp = tmp->next;
	}
	tmp = token;
	return (del_quotes_redir(tmp));
}
