/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/06 16:26:43 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	print_prompt(t_data *data)
{
	while (1)
	{
		//ctrl-c		
		signal(SIGINT, sig_manage);
		//ctrl-backslash
		signal(SIGQUIT, sig_manage);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data);
		printf("input = %s\n", data->input);
		data->trimmed = ft_strtrim(data->input, " ");
		printf("trimmed = %s\n", data->trimmed);
		if (data->input != NULL && ft_strlen(data->input) != 0)
			add_history(data->input);
	}
	return (0);
}
