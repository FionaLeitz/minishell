/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:35:21 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/08 16:18:41 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_data(t_data *data)
{
	data->input = NULL;
	data->trimmed =  NULL;
	data->head = NULL;
	data->i = 0;
}

void	init_token(t_token *token)
{
	token->type = 0;
	token->value = NULL;
	token->next = NULL;
	token->prev = NULL;
}
