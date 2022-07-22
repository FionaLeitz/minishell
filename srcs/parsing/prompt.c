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

void	ft_cut(t_data *data)
{
	t_token	*tmp;
	char	*save;

	first_pipe_cut(data);
	tmp = data->head;
	while (tmp)
	{
		save = ft_strtrim(tmp->value, " \t\n\v\f\r");
		free(tmp->value);
		tmp->value = save;
		tmp = tmp->next;
	}
	tmp = data->head;
	while (tmp)
	{
		count_red(data, tmp);
		tmp = tmp->next;
	}
// gerer ici le remplacement des $
// COMPLIQUE !
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
	(void)params;
	char **tabs;

	while (1)
	{
		init_data(data);
		//ctrl-c		
		signal(SIGINT, sig_manage);
		//ctrl-backslash
		signal(SIGQUIT, sig_manage);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data);
		if (syntax_check(data) == 0)
			ft_cut(data);
		/*tmp = data->head;
		while (tmp)
		{
			params->env = ft_select_builtin(tmp, params);
			tmp = tmp->next;
		}*/
//			printf("Error parsing\n");
//////////////////////////////
		tmp = data->head;
		tabs = red_tab(tmp, data);
		while (tmp)
		{
			printf("args: \n");
			print_table(tmp->args);
			printf("-----------\n");
			printf("red: \n");
			print_table(tmp->red);
			printf("-----------\n");
			printf("red_tab: \n");
			print_table(tabs);
			tmp = tmp->next;
		}
/////////////////////////////
		free_struct(data);
	}
	rl_clear_history();
	return (0);
}
