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

// count words
int	ft_count_words(t_data *data, char *s)
{
	int		count;
	char	quote;

	count = 0;
	data->i = 0;
	while (s[data->i] != '\0')
	{
		while (s[data->i] != '\0' && ft_space(s[data->i]) != 0)
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
		while (s[data->i] != '\0' && ft_space(s[data->i]) == 0)
			data->i++;
	}
	return (count);
}

// count characters in word
int	in_create_tab(char *str, int *i)
{
	char	quote;
	int		count;

	count = 0;
	while (str[i[0]] != '\0' && ft_space(
			str[i[0]]) != 0)
	{
		if (str[i[0]] == '\'' || str[i[0]] == '\"')
		{
			quote = str[i[0]];
			count++;
			i[0]++;
			while (str[i[0]] != quote)
			{
				count++;
				i[0]++;
			}
		}
		i[0]++;
		count++;
	}
	return (count);
}

// create char **args for every token, separating every "word"
int	create_tab(t_data *data, t_token *token)
{
	int		i;
	int		count;
	int		j;
	int		k;
	int		tmp;

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
		count = in_create_tab(token->value, &i);
		if (ft_space(token->value[i]) == 0
			|| token->value[i] == '\0')
			token->args[j++] = ft_strndup(&token->value[tmp], count);
		while (token->value[i] != '\0' && ft_space(
				token->value[i]) == 0)
			i++;
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
