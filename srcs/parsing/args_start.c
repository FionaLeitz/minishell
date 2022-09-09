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
				while (s[data->i] && s[data->i] != quote)
					data->i++;
				if (s[data->i] == '\0')
					break ;
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
			while (str[i[0]] && str[i[0]] != quote)
			{
				count++;
				i[0]++;
			}
		}
		if (str[i[0]])
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
		return (set_error_malloc("parse\n"));
	token->args[k] = NULL;
	while (j < k)
	{
		tmp = i;
		count = in_create_tab(token->value, &i);
		if (ft_space(token->value[i]) == 0
			|| token->value[i] == '\0')
		{
			token->args[j++] = ft_strndup(&token->value[tmp], count);
			if (token->args[j - 1] == NULL)
				return (set_error_malloc("parse\n"));
		}
		while (token->value[i] != '\0'
			&& ft_space(token->value[i]) == 0)
			i++;
	}
	return (0);
}
