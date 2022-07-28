/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:39:29 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 14:44:29 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if next quote exist
int	get_next_quote(char quote, t_data *data)
{
	while (data->trimmed[data->i] && data->trimmed[data->i] != quote)
		data->i++;
	if (data->trimmed[data->i] == quote)
		return (0);
	return (-1);
}

// check if quotes opened are clsoed correctly
int	check_quotes(t_data *data)
{
	char	quote;

	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quote = data->trimmed[data->i];
			data->i++;
			if (get_next_quote(quote, data) == -1)
				return (-1);
		}
		data->i++;
	}
	return (0);
}
