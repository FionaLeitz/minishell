/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:08:05 by fleitz            #+#    #+#             */
/*   Updated: 2022/09/12 10:08:11 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// trim and send every token in function f
static int	in_cut(t_data *data, t_token *token, int (f)(t_data*, t_token*))
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

// replace quotes
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

// use replace_quotes on every string in tab
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

	if (pipe_cut(data) == -1)
		return (-1);
	tmp = data->head;
	if (in_cut(data, tmp, create_red_tab) == -1)
		return (-1);
	tmp = data->head;
	if (expand(tmp, data, params) == -1)
		return (-1);
	tmp = data->head;
	if (in_cut(data, tmp, create_arg_tab) == -1)
		return (-1);
	tmp = data->head;
	if (del_quotes(tmp, params) == -1)
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
