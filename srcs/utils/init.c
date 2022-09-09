/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:35:21 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/15 16:34:22 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// put every variable in data to NULL
void	init_data(t_data *data)
{
	data->input = NULL;
	data->trimmed = NULL;
	data->head = NULL;
	data->pid = NULL;
	data->pipe_fd = NULL;
	data->i = 0;
}

// put every variable in token to NULL
void	init_token(t_token *token)
{
	token->value = NULL;
	token->args = NULL;
	token->red = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->fds[0] = 0;
	token->fds[1] = 1;
}
