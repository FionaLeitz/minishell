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

int	print_prompt(t_data *data)
{
	t_token *tmp;

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
		data->trimmed = ft_strtrim(data->input, " ");
		printf("trimmed = %s\n", data->trimmed);
		if (data->input != NULL && ft_strlen(data->input) != 0)
			add_history(data->input);
		if (check_string(data) == -1)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (2);
		}
		if (check_string(data) == -1)
		{
			printf("minishell: quotes are unclosed\n");
			return (2);
		}
		first_pipe_cut(data);
		tmp = data->head;
		while (tmp)
		{
			printf("%s = %d\n", tmp->value, ft_count_words(data, tmp->value, ' '));
			tmp = tmp->next;
		}
	}
	return (0);
}
