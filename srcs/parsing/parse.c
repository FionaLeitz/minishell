/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:47:40 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/18 14:23:40 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// jump quotes
void	jump_quotes(char *str, t_data *data)
{
	char	quote;
	int		save;

	quote = str[data->i];
	data->i++;
	save = data->i;
	while (str[data->i] != '\0' && str[data->i] != quote)
		data->i++;
	if (str[data->i] == '\0')
		data->i = save;
}

// check error with pipes
int	check_string(t_data *data)
{
	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
			jump_quotes(data->trimmed, data);
		else if (data->trimmed[data->i] == '|')
		{
			data->i++;
			while (data->trimmed[data->i]
				&& ft_space(data->trimmed[data->i]) == 0)
				data->i++;
			if (data->trimmed[data->i] == '|' || data->trimmed[data->i] == '\0')
			{
				ft_putstr_fd("minishell: syntax error ", 2);
				ft_putstr_fd("near unexpected token `|'\n", 2);
				return (-1);
			}
		}
		if (check_redir(data) == -1)
			return (-1);
		if (data->trimmed[data->i] == '\0')
			break ;
		data->i++;
	}
	return (0);
}

// parse errors
int	syntax_check(t_data *data)
{
	if (data->input != NULL && ft_strlen(data->input) != 0)
		add_history(data->input);
	data->trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
	if (check_quotes(data) == -1)
	{
		ft_putstr_fd("minishell: quotes are unclosed\n", 2);
		return (-1);
	}
	if (data->trimmed[0] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (2);
	}
	if (check_string(data) == -1)
		return (2);
	return (0);
}

// create command and argument
int	pipe_cut(t_data *data)
{
	int		count;
	char	quote;

	data->i = 0;
	count = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quote = data->trimmed[data->i];
			data->i++;
			get_next_quote(quote, data);
		}
		else if (data->trimmed[data->i] == '|')
		{
			if (push_back(data, ft_strndup(&data->trimmed[count],
						data->i - count)) == -1)
				return (-1);
			count = data->i + 1;
		}
		data->i++;
	}
	if (push_back(data, ft_strdup(&data->trimmed[count])) == -1)
		return (-1);
	return (0);
}
