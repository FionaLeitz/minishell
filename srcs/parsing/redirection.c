/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 11:44:53 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/03 11:44:56 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// print different redirection error
static int	print_error_redir(char *str, char c)
{
	printf("minishell: syntax error near unexpected token `%s%c'\n", str, c);
	return (-1);
}

// in check_redir
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

// check error with redirection's characteres
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

// in count_red, create char **red (redirection's list)
int	get_red(t_data *data, t_token *token, int count)
{
	int		j;
	int		save;
	char	quote;

	j = 0;
	data->i = 0;
	token->red[count] = NULL;
	while (j < count)
	{
		while (token->value[data->i] != '>' && token->value[data->i] != '<')
		{
			if (token->value[data->i] == '\'' || token->value[data->i] == '\"')
			{
				quote = token->value[data->i];
				data->i++;
				while (token->value[data->i] != quote)
					data->i++;
			}
			data->i++;
		}
		save = data->i;
		data->i++;
		while (ft_space(token->value[data->i]) == 0)
			data->i++;
		if (token->value[data->i] == '>' || token->value[data->i] == '<')
			data->i++;
		while (ft_space(token->value[data->i]) == 0)
			data->i++;
		while (token->value[data->i] != '\0' && ft_space(
				token->value[data->i]) != 0 && token->value[data->i] != '>'
			&& token->value[data->i] != '<')
		{
			if (token->value[data->i] == '\'' || token->value[data->i] == '\"')
			{
				quote = token->value[data->i];
				data->i++;
				while (token->value[data->i] != quote)
					data->i++;
			}
			data->i++;
		}
		token->red[j] = ft_strndup(&token->value[save], data->i - save);
		ft_memcpy(&token->value[save], &token->value[data->i],
			ft_strlen(&token->value[save]));
		data->i = save;
		j++;
	}
	return (0);
}

// count redirection's number and create char **red (redirection's list)
int	count_red(t_data *data, t_token *token)
{
	int		count;
	char	quote;

	count = 0;
	data->i = 0;
	while (token->value[data->i] != '\0')
	{
		if (token->value[data->i] == '\'' || token->value[data->i] == '\"')
		{
			quote = token->value[data->i];
			data->i++;
			while (token->value[data->i] != quote)
				data->i++;
		}
		else if (token->value[data->i] == '>' || token->value[data->i] == '<')
		{
			if (token->value[data->i] == token->value[data->i + 1])
				data->i++;
			count++;
		}
		data->i++;
	}
	token->red = malloc(sizeof(char *) * (count + 1));
	if (token->red == NULL)
		return (-1);
	if (get_red(data, token, count) != 0)
		return (-1);
	return (0);
}
