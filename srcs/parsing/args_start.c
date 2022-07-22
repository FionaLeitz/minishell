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

int	ft_count_words(t_data *data, char *s/*, char c*/)
{
	int		count;
	char	quote;

	count = 0;
	data->i = 0;
	while (s[data->i] != '\0')
	{
		while (s[data->i] != '\0' && ft_check_whitespace(s[data->i]) != 0/*s[data->i] != c*/)
		{
			if (s[data->i] == '\'' || s[data->i] == '\"')
			{
				quote = s[data->i];
				data->i++;
				while (s[data->i] != quote)
					data->i++;
			}
			data->i++;	
		}
		count++;
		while (s[data->i] != '\0' && ft_check_whitespace(s[data->i]) == 0)
			data->i++;
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
	k = ft_count_words(data, token->value);
	token->args = malloc(sizeof(char *) * (k + 1));
	if (!token->args)
		return (-1);
	token->args[k] = NULL;
	while (j < k)
	{
		tmp = i;
		count = 0;
		while (token->value[i] != '\0' && ft_check_whitespace(token->value[i]) != 0/*token->value[i] != ' '*/)
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
		if (/*token->value[i] == ' ' */ft_check_whitespace(token->value[i]) == 0|| token->value[i] == '\0')
		{
			token->args[j] = ft_strndup(&token->value[tmp], count);
			j++;
		}
		while (token->value[i] != '\0' && ft_check_whitespace(token->value[i]) == 0/*token->value[i] == ' '*/)
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

		while (ft_check_whitespace(token->value[data->i]) == 0)	
			data->i++;
		if (token->value[data->i] == '>' || token->value[data->i] == '<')
				data->i++;

		while (ft_check_whitespace(token->value[data->i]) == 0)
			data->i++;
		while (token->value[data->i] != '\0' &&
			ft_check_whitespace(token->value[data->i]) != 0 &&
			token->value[data->i] != '>' && token->value[data->i] != '<')
		{
			if (token->value[data->i] == '\'' || token->value[data->i] == '\"')
			{
				quote = token->value[data->i];
				data->i++;
				while (token->value[data->i] != quote)
					data->i++;
			}
		//	else
				data->i++;	
		}
		token->red[j] = ft_strndup(&token->value[save], data->i - save + 1);
		ft_memcpy(&token->value[save], &token->value[data->i], ft_strlen(&token->value[save]));
		data->i = save;
		j++;
	}
	return (0);
}

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