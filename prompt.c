/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/19 18:23:52 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// only if problem with malloc
int	ft_get_out(t_data *data)
{
	free_struct(data);
	rl_clear_history();
	g_exit_st = 12;
	return (12);
}

int	can_continue(t_data *data, t_params *params)
{
	t_token	*tmp;

	if (ft_cut(data, params) == -1)
		return (ft_get_out(data));
	tmp = data->head;
	if (params->error == -2)
		return (0);
	if (ft_execute(tmp, params) == -1)
		return (ft_get_out(data));
	return (0);
}

// give the prompt, get readline, parses and execution's fonctions
int	prompt(t_data *data, t_params *params)
{
	int		syntax;

	params->data = data;
	while (1)
	{
		params->old_fd[0] = -1;
		params->old_fd[1] = -1;
		params->error = 0;
		init_data(data);
		ft_signals(DEFAULT);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data, params);
		syntax = syntax_check(data);
		if (syntax == 2)
			only_heredocs(data);
		if (syntax != -1)
			if (can_continue(data, params) == 12)
				return (12);
		free_struct(data);
	}
	rl_clear_history();
	return (0);
}
