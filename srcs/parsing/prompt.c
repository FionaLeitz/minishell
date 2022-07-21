/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/14 10:24:53 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_table2(char **table, int j)
{
	int	i;

	i = 0;
	if (j == 2)
		printf("red :\n");
	if (j == 1)
		printf("args :\n");
	while (table[i])
	{
		if (table[i] == NULL)
			printf("C'est vide\n");
		printf("%s", table[i]);
		printf("\n");
		i++;
	}
	printf("------\n");
}

int	syntax_check(t_data *data)
{
	data->trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
	if (data->trimmed[0] == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_struct(data);
		return (-1);
	}
	if (data->input != NULL && ft_strlen(data->input) != 0)
		add_history(data->input);
	//rl_clear_history();
	if (check_string(data) == -1)
	{
		free_struct(data);
		return (-1);
	}
	if (check_quotes(data) == -1)
	{
		printf("minishell: quotes are unclosed\n");
		free_struct(data);
		return (-1);
	}
	return (0);
}
/*
int	replace_var(t_data *data, t_params *params)
{
	int	save;
	(void)params;
	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'')
			jump_quotes(data->trimmed, data);
		if (data->trimmed[data->i] == '$')
		{
			save = data->i;
			while (data->trimmed[data->i] && ft_check_whitespace(data->trimmed[data->i]) != 0)
				data->i++;
		}
		data->i++;
	}
	return (0);
}*/

void	ft_cut(t_data *data, t_params *params)
{
	t_token	*tmp;
	char	*save;

	(void)params;

	first_pipe_cut(data);
	tmp = data->head;
	while (tmp)
	{
		save = ft_strtrim(tmp->value, " \t\n\v\f\r");
		free(tmp->value);
		tmp->value = save;
		tmp = tmp->next;
	}
//	replace_var(data, params);
//	if (replace_var() == -1)
//		return (-1);
	tmp = data->head;
	while (tmp)
	{
		count_red(data, tmp);
		tmp = tmp->next;
	}
	tmp = data->head;
	while (tmp)
	{
		create_tab(data, tmp);
		tmp = tmp->next;
	}
	tmp = data->head;
	del_quotes(tmp);
}

int	print_prompt(t_data *data, t_params *params)
{
	t_token	*tmp;

	while (1)
	{
		init_data(data);
		signal(SIGINT, sig_manage);
		signal(SIGQUIT, sig_manage);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data);
		if (syntax_check(data) == 0)
			ft_cut(data, params);
		tmp = data->head;
		while (tmp && tmp->args[0])
		{
			printf("At the end :\n");
			print_table(tmp->args);
			printf("----------\n");
			print_table(tmp->red);
//			params->env = ft_select_builtin(tmp, params);
			tmp = tmp->next;
		}
		free_struct(data);
	}
	rl_clear_history();
	return (0);
}
