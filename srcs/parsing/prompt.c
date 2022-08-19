/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/18 15:02:48 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// parse errors
int	syntax_check(t_data *data)
{
	if (data->input != NULL && ft_strlen(data->input) != 0)
		add_history(data->input);
	data->trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
	if (data->trimmed[0] == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_struct(data);
		return (2);
	}
	if (check_string(data) == -1)
	{
		free_struct(data);
		return (2);
	}
	if (check_quotes(data) == -1)
	{
		printf("minishell: quotes are unclosed\n");
		free_struct(data);
		return (2);
	}
	return (0);
}

// start separating pipes
void	ft_cut(t_data *data, t_params *params)
{
	t_token	*tmp;
	char	*s;

	first_pipe_cut(data);
	tmp = data->head;
	while (tmp)
	{
		s = ft_strtrim(tmp->value, " \t\n\v\f\r");
		free(tmp->value);
		tmp->value = s;
		tmp = tmp->next;
	}
	tmp = data->head;
	while (tmp)
	{
		count_red(data, tmp);
		tmp = tmp->next;
	}
	tmp = data->head;
	if (replace_var(tmp, data, params) == -1)
	{
		printf("ERROR REPLACE_VAR\n");
		return ;
	}
	tmp = data->head;
	while (tmp)
	{
		s = ft_strtrim(tmp->value, " \t\n\v\f\r");
		free(tmp->value);
		tmp->value = s;
		create_tab(data, tmp);
		tmp = tmp->next;
	}
	tmp = data->head;
	del_quotes(tmp);
	tmp = data->head;
	while (tmp)
	{
		ft_redirection(tmp->red, params, data, tmp);
		tmp = tmp->next;
	}
}

// give the prompt, get readline, parses and execution's fonctions
int	print_prompt(t_data *data, t_params *params)
{
	t_token	*tmp;

	params->data = data;
	while (1)
	{
		init_data(data);
		ft_signals(DEFAULT);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data, params);
		if (syntax_check(data) != 0)
			exit_st = 2;
		else
		{
			ft_cut(data, params);
			tmp = data->head;
			ft_execute(tmp, params);
		}
		free_struct(data);
	}
	free_params(params);
	rl_clear_history();
	return (0);
}
