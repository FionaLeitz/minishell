/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 10:53:47 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 10:36:34 by masamoil         ###   ########.fr       */
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

// handle ctrl-\ and ctrl-c
void	sig_manage(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}		
	if (signal == SIGQUIT)
		printf("\b\b  \b\b");
	return ;
}

// handle ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	printf("exit\n");
	free_params(params);
	free_struct(data);
	exit(0);
}
