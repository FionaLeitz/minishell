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

void	in_cut(t_data *data, t_token *token, int (f)(t_data*, t_token*))
{
	char	*s;

	while (token)
	{
		token->fds[0] = 0;
		token->fds[1] = 1;
		s = ft_strtrim(token->value, " \t\n\v\f\r");
		free(token->value);
		token->value = s;
		f(data, token);
		token = token->next;
	}
}

// start separating pipes
void	ft_cut(t_data *data, t_params *params)
{
	t_token	*tmp;
	int		i;
	int		j;

	first_pipe_cut(data);
	tmp = data->head;
	in_cut(data, tmp, count_red);
	tmp = data->head;
	replace_var(tmp, data, params);
	tmp = data->head;
	in_cut(data, tmp, create_tab);
	tmp = data->head;
	del_quotes(tmp);
	tmp = data->head;
	while (tmp)
	{
		ft_redirection(tmp->red, params, tmp);
		tmp = tmp->next;
	}
	tmp = data->head;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			j = 0;
			dprintf(2, "args before = -%s-\n", tmp->args[i]);
			while (tmp->args[i][j])
			{
				if (tmp->args[i][j] == -1)
					tmp->args[i][j] = '\'';
				else if (tmp->args[i][j] == -2)
					tmp->args[i][j] = '\"';
				j++;
			}
			dprintf(2, "args = -%s-\n", tmp->args[i]);
			i++;
		}
		i = 0;
		while (tmp->red[i])
		{
			j = 0;
			while (tmp->red[i][j])
			{
				if (tmp->red[i][j] == -1)
					tmp->red[i][j] = '\'';
				else if (tmp->red[i][j] == -2)
					tmp->red[i][j] = '\"';
				j++;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

static int	modify_line(t_data *data, int nbr)
{
	int	save;

	save = 0;
	while (ft_space(data->trimmed[data->i]) == 0)
	{
		data->i++;
		nbr++;
	}
	while (ft_space(data->trimmed[data->i]) != 0)
	{
		save = data->i;
		if (data->trimmed[data->i] == '\''
			|| data->trimmed[data->i] == '\"')
			jump_quotes(data->trimmed, data);
		else
			data->i++;
		nbr += data->i - save;
	}
	return (nbr);
}

static void	only_heredocs(t_data *data)
{
	int	save;
	int	save2;

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
			save += modify_line(data, save2);
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
			only_heredocs(data);
			dprintf(2, "data->trimmed = -%s-\n", data->trimmed);
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
