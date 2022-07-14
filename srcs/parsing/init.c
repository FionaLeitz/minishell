/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:35:21 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 16:40:46 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_data(t_data *data)
{
	data->input = NULL;
	data->trimmed = NULL;
	data->head = NULL;
	data->i = 0;
}

void	init_token(t_token *token)
{
	token->value = NULL;
	token->args = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->args = NULL;
}
