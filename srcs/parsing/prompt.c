/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 11:29:08 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_table(char **table)
{
	int	i;

	i = 0;
	while (table[i])
	{
		if (table[i] == NULL)
			printf("C'est vide\n");
		ft_printf(table[i]);
		ft_printf("\n");
		i++;
	}
}

int	print_prompt(t_data *data)
{
	t_token *tmp;
	char	*save;

	while (1)
	{
		//init_token(token_list);
		init_data(data);
		//ctrl-c		
		signal(SIGINT, sig_manage);
		//ctrl-backslash
		signal(SIGQUIT, sig_manage);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data);
		data->trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
		printf("trimmed = %s\n", data->trimmed);
		if (data->input != NULL && ft_strlen(data->input) != 0)
			add_history(data->input);
		if (check_string(data) == -1)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (2);
		}
		if (check_quotes(data) == -1)
		{
			printf("minishell: quotes are unclosed\n");
			return (2);
		}
		first_pipe_cut(data);
		tmp = data->head;
		while (tmp)
		{
			save = ft_strtrim(tmp->value, " ");
			free(tmp->value);
			tmp->value = save;
			tmp = tmp->next;
		}
		tmp = data->head;
		while (tmp)
		{
			printf("%s = %d\n", tmp->value, ft_count_words(data, tmp->value, ' '));
			tmp = tmp->next;
		}
		tmp = data->head;
		while (tmp)
		{
			create_tab(data, tmp);
			tmp = tmp->next;
		}
//		printf("%p\n", data->head->args);
//		printf("%p\n", data->head->args[0]);
		tmp = data->head;
		while (tmp)
		{
			print_table(tmp->args);
			tmp = tmp->next;
		}
//		printf("No of el = %d\n", ft_size(data->head));
	}
	return (0);
}

