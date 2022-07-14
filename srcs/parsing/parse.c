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
	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '|')
		{
			data->i++;
			while (data->trimmed[data->i]
				&& ft_check_whitespace(data->trimmed[data->i]) == 0)
				data->i++;
			if (data->trimmed[data->i] == '|')
				return (-1);
		}
		data->i++;
	}
	return (0);
}
