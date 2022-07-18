/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:47:40 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 18:07:01 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_string(t_data *data)
{
	char	quote;

	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quote = data->trimmed[data->i];
			data->i++;
			while (data->trimmed[data->i] != quote)
				data->i++;
		}
		else if (data->trimmed[data->i] == '|')
		{
			data->i++;
			while (data->trimmed[data->i]
				&& ft_check_whitespace(data->trimmed[data->i]) == 0)
				data->i++;
			if (data->trimmed[data->i] == '|' || data->trimmed[data->i] == '\0')
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (-1);
			}
		}
		if (check_redir(data) == -1)
			return (-1);
		data->i++;
	}
	return (0);
}

int	check_redir(t_data *data)
{
	char	redir;

	if (data->trimmed[data->i] == '<' || data->trimmed[data->i] == '>')
	{
		redir = data->trimmed[data->i];
		data->i++;
		while (data->trimmed[data->i]
			&& ft_check_whitespace(data->trimmed[data->i]) == 0)
			data->i++;
		if (data->trimmed[data->i] == '\0')
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (-1);
		}
		if (data->trimmed[data->i] == '<' || data->trimmed[data->i] == '>')
		{
			if (data->trimmed[data->i] != redir)
			{
				printf("minishell: syntax error near unexpected token `%c'\n", data->trimmed[data->i]);
				return (-1);
			}
			redir = data->trimmed[data->i];
			data->i++;
		}
		while (data->trimmed[data->i]
			&& ft_check_whitespace(data->trimmed[data->i]) == 0)
			data->i++;
		if (data->trimmed[data->i] == '\0')
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (-1);
		}
		if (data->trimmed[data->i] == '<' || data->trimmed[data->i] == '>')
		{
			printf("minishell: syntax error near unexpected token `%c'\n", redir);
			return (-1);
		}
	}
	return (0);
}