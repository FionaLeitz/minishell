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

#include "../../minishell.h"

// parse errors
int	syntax_check(t_data *data)
{
	if (data->input != NULL && ft_strlen(data->input) != 0)
		add_history(data->input);
	data->trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
	if (check_quotes(data) == -1)
	{
		printf("minishell: quotes are unclosed\n");
		return (2);
	}
	if (data->trimmed[0] == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (2);
	}
	if (check_string(data) == -1)
		return (2);
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

void	modify_line(t_data *data)
{
	int	save;
	int	save2;
	int	save3;

	data->i -= 1;
	data->trimmed[data->i] = '\0';
	data->trimmed[data->i + 1] = '\0';
	data->i = 0;
	save = 0;
	while (data->trimmed[data->i])
	{
		save2 = 2;
		if (data->trimmed[data->i] == '<' && data->trimmed[data->i + 1] == '<')
		{
			ft_memcpy(&data->trimmed[save], &data->trimmed[data->i],
				ft_strlen(&data->trimmed[data->i]) + 1);
			data->i = save + 2;
			while (ft_space(data->trimmed[data->i]) == 0)
			{
				data->i++;
				save2++;
			}
			while (ft_space(data->trimmed[data->i]) != 0)
			{
				save3 = data->i;
				if (data->trimmed[data->i] == '\'' ||
					data->trimmed[data->i] == '\"')
					jump_quotes(data->trimmed, data);
				else
					data->i++;
				save2 += data->i - save3;
			}
			save += save2;
		}
		data->i++;
	}
	data->trimmed[save] = '\0';
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
		{
		 	modify_line(data);
			g_exit_st = 2;
		}
		ft_cut(data, params);
		tmp = data->head;
		ft_execute(tmp, params);
		free_struct(data);
	}
	free_params(params);
	rl_clear_history();
	return (0);
}
