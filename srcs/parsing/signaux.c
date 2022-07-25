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

void	free_struct(t_data *data, t_params *params)
{
	t_token		*tmp;
	t_export	*exp;

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
	free_table(params->env);
	while (params->export)
	{
		exp = params->export;
		free(exp->name);
		free(exp->value);
		params->export = params->export->next;
		free(exp);
	}
	init_data(data);
}

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

void	ft_exit_d(t_data *data)
{
	printf("exit\n");
	(void)data;
	exit(0);
}
