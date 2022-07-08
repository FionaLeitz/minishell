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

t_token	*check_type(t_data *data, t_token *token_list)
{
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == ' ')
			data->i++;
		if (data->trimmed[data->i] == '<')
			l_redirect(data, token_list);
		//if (data->trimmed[data->i] == '>')
		//	r_redirect(data, );
		/*if (data->trimmed[i] == '"')
			printf("double quotes");*/
		if (data->trimmed[data->i] == '|')
			pushBack(data, "|", PIPE);
		data->i++;
	//	else
	//		printf("no token found");
	}
	return (token_list);
}
