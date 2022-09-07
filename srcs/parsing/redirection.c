/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 11:44:53 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/08 11:56:45 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// print different redirection error
static int	print_error_redir(char *str, char c)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\'\n", 2);
	return (-1);
}

// in check_redir
char	first_redir(char *str, t_data *data)
{
	char	redir;

	redir = str[data->i];
	data->i++;
	if (str[data->i] == '\0' || str[data->i] == '|')
	{
		if (str[data->i] == '|')
			print_error_redir("\0", '|');
		else
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
		if (data->trimmed[data->i] == '|')
			return (print_error_redir("\0", '|'));
		if (data->trimmed[data->i] == '<' || data->trimmed[data->i] == '>')
			return (print_error_redir("\0", redir));
	}
	return (0);
}

static void	locate_redir(t_data *data, t_token *token)
{
	char	quote;

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
}

static void	suppress_useless(t_data *data, t_token *token)
{
	int		tmp;
	char	quote;

	tmp = 0;
	while (ft_space(token->value[data->i]) == 0)
	{
		data->i++;
		tmp++;
	}
	ft_memcpy(&token->value[data->i - tmp], &token->value[data->i],
		ft_strlen(&token->value[data->i]) + 1);
	data->i -= tmp;
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
}

// in count_red, create char **red (redirection's list)
int	get_red(t_data *data, t_token *token, int count)
{
	int		j;
	int		save;

	j = 0;
	data->i = 0;
	token->red[count] = NULL;
	while (j < count)
	{
		locate_redir(data, token);
		save = data->i;
		data->i++;
		if (token->value[data->i] == '>' || token->value[data->i] == '<')
			data->i++;
		suppress_useless(data, token);
		token->red[j] = ft_strndup(&token->value[save], data->i - save);
		if (token->red[j] == NULL)
			return (set_error_malloc("parse\n"));
		ft_memcpy(&token->value[save], &token->value[data->i],
			ft_strlen(&token->value[data->i]) + 1);
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
			quote = token->value[data->i++];
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
		return (set_error_malloc("parse\n"));
	if (get_red(data, token, count) == -1)
		return (-1);
	return (0);
}
