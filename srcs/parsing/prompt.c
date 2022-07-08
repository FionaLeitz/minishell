/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/08 13:17:04 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	print_prompt(t_data *data)
{
	t_token *token_list;

	token_list = NULL;
	while (1)
	{
		//init_token(token_list);
		init_data(data);
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
		token_list = check_type(data, token_list);
		preview(data);
	}
	return (0);
}
