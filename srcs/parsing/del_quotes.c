/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   q_d.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 16:29:12 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 16:29:27 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	suppress_quotes(char *str)
{
	int		j;
	char	*tmp;

	j = 0;
	while (str[j] == '>' || str[j] == '<')
		j++;
	if (ft_space(str[j]) == 0 || ft_space(str[ft_strlen(str) - 1]) == 0)
	{
		tmp = malloc(sizeof(char) * ft_strlen(str) + 3);
		if (tmp == NULL)
			return (set_error_malloc("parse\n"));
		ft_bzero(tmp, ft_strlen(str) + 3);
		tmp[0] = str[0];
		tmp[1] = str[1];
		tmp[j] = '\'';
		ft_strcat(tmp, &str[j]);
		tmp[ft_strlen(tmp)] = '\'';
		free(str);
		str = tmp;
	}
	replace_quotes(str);
	return (0);
}

// find $ in redirection
static int	expand_redir(char **str, t_data *data, t_params *params)
{
	int		quote[3];

	quote[0] = 0;
	quote[2] = 0;
	data->i = -1;
	while (str[0][++data->i])
	{
		if (str[0][data->i] == '\"')
			quote[2]++;
		if (str[0][data->i] == '\'' && quote[2] % 2 == 0)
			quote[0]++;
		if (str[0][data->i] == '\'' && quote[0] % 2 != 0)
		{
			jump_quotes(str[0], data);
			quote[0]++;
		}
		if (str[0][data->i] == '$')
		{
			if (if_dollar(str, data, params, quote) == -1)
				return (-1);
		}
	}
	return (0);
}

static void	find_quotes(char **red, int *i, int *j)
{
	while (red[*i][++(*j)] != '\0')
	{
		if (red[*i][0] == '<' && red[*i][1] == '<')
			break ;
		if (red[*i][*j] == '\'' || red[*i][*j] == '\"')
			*j = move_to_suppress(red[*i], *j);
		if (red[*i][*j] == '\0')
			break ;
		if (red[*i][*j] == '\'' || red[*i][*j] == '\"')
			(*j)--;
	}
}

// find suppressable quotes in redirections
int	del_quotes_redir(t_token *token, t_params *params)
{
	int		i;
	int		j;

	while (token)
	{
		i = -1;
		while (token->red[++i])
		{
			if (!(token->red[i][0] == '<' && token->red[i][1] == '<'))
				expand_redir(&token->red[i], params->data, params);
			j = -1;
			find_quotes(token->red, &i, &j);
			if (suppress_quotes(token->red[i]) == -1)
				return (-1);
		}
		token = token->next;
	}
	return (0);
}
