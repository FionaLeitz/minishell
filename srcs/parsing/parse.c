/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:47:40 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/08 17:33:58 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// jump quotes
void	jump_quotes(char *str, t_data *data)
{
	char	quote;

	quote = str[data->i];
	data->i++;
	while (str[data->i] != '\0' && str[data->i] != quote)
		data->i++;
} 

// check error with pipes
int	check_string(t_data *data)
{
	data->i = -1;
	while (data->trimmed[++data->i])
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
				printf("minishell: syntax error near unexpected token `|'\n");
				return (-1);
			}
		}
		if (check_redir(data) == -1)
			return (-1);
	}
	return (0);
}

// create first element, which is the first command and argument before pipe
int	first_pipe_cut(t_data *data)
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
			push_back(data, ft_strndup(&data->trimmed[count], data->i - count));
			count = data->i + 1;
		}
		data->i++;
	}
	push_back(data, ft_strdup(&data->trimmed[count]));
	return (0);
}
