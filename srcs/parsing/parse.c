/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:47:40 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/08 15:03:16 by masamoil         ###   ########.fr       */
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
			while (data->trimmed[data->i] && data->trimmed[data->i] == ' ')
				data->i++;
			if (data->trimmed[data->i] == '|')
				return (-1);
		}
		data->i++;
	}
	return (0);
}
