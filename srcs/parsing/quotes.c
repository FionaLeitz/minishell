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
	while (str[j] != quote)
		j++;
	ft_memcpy(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
	return (j);
}

// find suppressable quotes in redirections
void	del_quotes_redir(t_token *token)
{
	int		i;
	int		j;
	char	*tmp;

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
			j = 0;
			while (token->red[i][j] == '>' || token->red[i][j] == '<')
				j++;
			if (ft_space(token->red[i][j]) == 0 || ft_space(token->red[i][
				ft_strlen(token->red[i]) - 1]) == 0)
			{
				tmp = malloc(sizeof(char) * ft_strlen(token->red[i]) + 3);
				ft_bzero(tmp, ft_strlen(token->red[i]) + 3);
				tmp[0] = token->red[i][0];
				tmp[1] = token->red[i][1];
				tmp[j] = '\'';
				ft_strcat(tmp, &token->red[i][j]);
				tmp[ft_strlen(tmp)] = '\'';
				free(token->red[i]);
				token->red[i] = tmp;
			}
		}
		token = token->next;
	}
}

// find suppressable quotes
void	del_quotes(t_token *token)
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
	del_quotes_redir(tmp);
	return ;
}
