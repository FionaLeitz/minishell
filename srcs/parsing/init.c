/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:35:21 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/09 18:19:43 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// put every variable in data to NULL
void	init_data(t_data *data)
{
	data->input = NULL;
	data->trimmed = NULL;
	data->head = NULL;
	data->i = 0;
	data->fd_in = dup(STDIN_FILENO);
	data->fd_out = dup(STDOUT_FILENO);
}

// put every variable in token to NULL
void	init_token(t_token *token)
{
	token->value = NULL;
	token->args = NULL;
	token->red = NULL;
	token->next = NULL;
	token->prev = NULL;
}
