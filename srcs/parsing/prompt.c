/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/04 13:47:48 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	print_prompt(t_data *data)
{
	while (1)
	{
		data->input = readline(PROMPT);
		printf("input = %s\n", data->input);
		if (!data->input)
			free(data->input);
		if (data->input != NULL && ft_strlen(data->input) != 0)
			add_history(data->input);
	}
	return (0);
}
