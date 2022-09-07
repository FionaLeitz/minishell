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
		ft_putstr_fd("minishell: quotes are unclosed\n", 2);
		return (2);
	}
	if (data->trimmed[0] == '|')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (2);
	}
	if (check_string(data) == -1)
		return (2);
	return (0);
}

int	in_cut(t_data *data, t_token *token, int (f)(t_data*, t_token*))
{
	char	*s;

	while (token)
	{
		token->fds[0] = 0;
		token->fds[1] = 1;
		s = ft_strtrim(token->value, " \t\n\v\f\r");
		free(token->value);
		token->value = s;
		if (f(data, token) == -1)
			return (-1);
		token = token->next;
	}
	return (0);
}

void	replace_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == -1)
			str[i] = '\'';
		else if (str[i] == -2)
			str[i] = '\"';
		i++;
	}
}

static void	give_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		replace_quotes(tab[i]);
}

// start separating pipes
int	ft_cut(t_data *data, t_params *params)
{
	t_token	*tmp;

	if (first_pipe_cut(data) == -1)
		return (-1);
	tmp = data->head;
	if (in_cut(data, tmp, count_red) == -1)
		return (-1);
	tmp = data->head;
	if (replace_var(tmp, data, params) == -1)
		return (-1);
	tmp = data->head;
	if (in_cut(data, tmp, create_tab) == -1)
		return (-1);
	tmp = data->head;
	if (del_quotes(tmp) == -1)
		return (-1);
	tmp = data->head;
	while (tmp)
	{
		ft_redirection(tmp->red, params, tmp);
		give_tab(tmp->args);
		give_tab(tmp->red);
		tmp = tmp->next;
	}
	return (0);
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

int	ft_get_out(t_data *data)
{
	free_struct(data);
	rl_clear_history();
	g_exit_st = 12;
	return (12);
}

// give the prompt, get readline, parses and execution's fonctions
int	print_prompt(t_data *data, t_params *params)
{
	t_token	*tmp;

	params->data = data;
	init_data(data);
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
			g_exit_st = 2;
		}
		if (ft_cut(data, params) == -1)
			return (ft_get_out(data));
		tmp = data->head;
		if (ft_execute(tmp, params) == -1)
			return (ft_get_out(data));
		free_struct(data);
	}
	rl_clear_history();
	return (0);
}
