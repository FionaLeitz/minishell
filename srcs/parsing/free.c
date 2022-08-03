/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:18:00 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/03 16:18:18 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// free data
void	free_struct(t_data *data)
{
	t_token	*tmp;

	if (data->input != NULL)
		free(data->input);
	if (data->trimmed != NULL)
		free(data->trimmed);
	while (data->head != NULL)
	{
		tmp = data->head;
		free(tmp->value);
		free_table(tmp->args);
		free_table(tmp->red);
		data->head = data->head->next;
		free(tmp);
	}
	init_data(data);
}

// free params
void	free_params(t_params *params)
{
	t_export	*tmp;

	free_table(params->env);
	while (params->export)
	{
		tmp = params->export;
		free(tmp->name);
		free(tmp->value);
		params->export = params->export->next;
		free(tmp);
	}
}