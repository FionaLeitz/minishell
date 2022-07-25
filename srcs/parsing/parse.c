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

void	jump_quotes(char *str, t_data *data)
{
	char	quote;

	quote = str[data->i];
	data->i++;
	while (str[data->i] != quote)
		data->i++;
}

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

static int	print_error_redir(char *str, char c)
{
	printf("minishell: syntax error near unexpected token `%s%c'\n", str, c);
	return (-1);
}

char	first_redir(char *str, t_data *data)
{
	char	redir;

	redir = str[data->i];
	data->i++;
	while (str[data->i]
		&& ft_space(str[data->i]) == 0)
		data->i++;
	if (str[data->i] == '\0')
	{
		print_error_redir("newline", '\0');
		return (-1);
	}
	if (str[data->i] == '<' || str[data->i] == '>')
	{
		if (str[data->i] != redir)
		{
			print_error_redir("\0", str[data->i]);
			return (-1);
		}
		redir = str[data->i];
		data->i++;
	}
	return (redir);
}

int	check_redir(t_data *data)
{
	char	redir;

	if (data->trimmed[data->i] == '<' || data->trimmed[data->i] == '>')
	{
		redir = first_redir(data->trimmed, data);
		if (redir == -1)
			return (-1);
		while (data->trimmed[data->i]
			&& ft_space(data->trimmed[data->i]) == 0)
			data->i++;
		if (data->trimmed[data->i] == '\0')
			return (print_error_redir("newline", '\0'));
		if (data->trimmed[data->i] == '<' || data->trimmed[data->i] == '>')
			return (print_error_redir("\0", redir));
	}
	return (0);
}
