/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 18:05:33 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 18:06:04 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	preview(data);
	return (0);
}

int	ft_count_words(t_data *data, char *s, char c)
{
	int		count;
	char	quote;

	count = 0;
	data->i = 0;
	while (s[data->i] != '\0')
	{
		if (s[data->i] == c)
			data->i++;
		else if (s[data->i] == '\'' || s[data->i] == '\"')
		{
			count++;
			quote = s[data->i];
			data->i++;
			while (s[data->i++] && s[data->i++] != quote)
			data->i++;
		}
		else
		{
			count++;
			while (s[data->i] != '\0' && s[data->i] != c)
				data->i++;
		}
	}
	return (count);
}

int	create_tab(t_data *data, t_token *token)
{
	int		i;
	int		count;
	int		j;
	int		k;
	int		tmp;
	char	quote;

	i = 0;
	j = 0;
	k = ft_count_words(data, token->value, ' ');
	token->args = malloc(sizeof(char *) * (k + 1));
	if (!token->args)
		return (-1);
	token->args[k] = NULL;
	while (j < k)
	{
		tmp = i;
		count = 0;
		while (token->value[i] != '\0' && token->value[i] != ' ')
		{
			if (token->value[i] == '\'' || token->value[i] == '\"')
			{
				quote = token->value[i];
				count++;
				i++;
				while (token->value[i] != quote)
				{
					count++;
					i++;
				}
			}
			i++;
			count++;
		}
		if (token->value[i] == ' ' || token->value[i] == '\0')
		{
			token->args[j] = ft_strndup(&token->value[tmp], count);
			j++;
		}
		while (token->value[i] != '\0' && token->value[i] == ' ')
			i++;
	}
	return (0);
}

int	del_quotes(t_token *token)
{
	char	quote;
	t_token	*tmp;
	int		i;
	int		j;

	tmp = token;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			j = 0;
			while (tmp->args[i][j] != '\0')
			{
				if (tmp->args[i][j] == '\'' || tmp->args[i][j] == '\"')
				{
					quote = tmp->args[i][j];
					ft_memcpy(&tmp->args[i][j], &tmp->args[i][j + 1], ft_strlen(&tmp->args[i][j]));
					while (tmp->args[i][j] != quote)
						j++;
					ft_memcpy(&tmp->args[i][j], &tmp->args[i][j + 1], ft_strlen(&tmp->args[i][j]));
				}
				if (tmp->args[i][j] == '\0')
					break ;
				j++;
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}
