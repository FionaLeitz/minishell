/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 10:54:26 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 11:38:21 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_quotes(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;	
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			count++;
		i++;
	}
	return (count);
}

int	check_quotes(t_data *data)
{	
	char	*quotes;
	int		count;

	count = 0;
	data->i = 0;
	quotes = malloc(sizeof(char) * (count_quotes(data->trimmed) + 1));
	if (quotes == NULL)
		return (-1);
	quotes[0] = '\0';
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quotes[count] = data->trimmed[data->i];
			count++;
			quotes[count] = '\0';
			if (quotes_reduce(quotes) == 0)
				count -= 2;
		}
		data->i++;
	}
	if (quotes[0] = '\0')
	{
		free(quotes);
		return (0);
	}
	return (-1);
}

int	quotes_reduce(char *quotes)
{
	int count;

	count = ft_strlen(quotes) - 1;
	if (count == 0)
		return (-1);
	if (quotes[count] == quotes[count - 1])
	{
		quotes[count] = '\0';
		quotes[count - 1] = '\0';
		return (0);
	}
	return (-1);
}

/*
int	quotes_trimmed(t_data *data)
{
	int	first;
	int	last;
	char *quotes;

	quotes = check_quotes(data);
	first = 0;
	last = ft_strlen(quotes) - 1;
	if (last % 2 == 0)
		return (-1);
	while (quotes[0] != '\0')
	{
		if (quotes[first] != quotes[last])
			return (-1);
		ft_memcpy(quotes, &quotes[1], last);
		last--;
		quotes[last] = '\0';
		last--;
	}
	return (0);
}*/
