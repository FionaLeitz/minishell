/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_fct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 11:42:33 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/02 11:40:03 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// find variable to replace $
char	*rep(char **env, char *str, int *quote, char *buff)
{
	int		count;

	if ((quote[1] == 0 || (quote[0] % 2 != 0 && str[0] == '\"')) && str[0] != '\"' && str[0] != '\'')
		return ("$");
	if (str[0] == '?')
	{
		ft_itoa_no_malloc(g_exit_st, buff);
		return (buff);
	}
	count = -1;
	while (env[++count])
	{
		if (ft_strncmp(env[count], str, quote[1]) == 0
			&& env[count][quote[1]] == '=')
			return (&env[count][quote[1] + 1]);
	}
	return (NULL);
}

// replace $
int	in_replace(char *str, int s, t_token *token, t_data *data)
{
	char	*tmp;

	if (str == NULL)
	{
		ft_memmove(&token->value[s], &token->value[data->i],
			ft_strlen(&token->value[data->i]) + 1);
		data->i = s - 1;
	}
	else
	{
		tmp = malloc(sizeof(char) * (s + ft_strlen(str)
					+ ft_strlen(&token->value[data->i]) + 1));
		if (tmp == NULL)
			return (-1);
		ft_bzero(tmp, s + ft_strlen(str)
			+ ft_strlen(&token->value[data->i]) + 1);
		tmp = ft_memmove(tmp, token->value, s);
		tmp = ft_strcat(tmp, str);
		tmp = ft_strcat(tmp, &token->value[data->i]);
		free(token->value);
		token->value = tmp;
		data->i = s + ft_strlen(str) - 1;
	}
	return (0);
}

static int	if_dollar(t_token *token, t_data *data, t_params *par, int *quote)
{
	int		s;
	char	*str;
	char	buff[12];

	s = data->i++;
	while (token->value[data->i] && ft_space(token->value[data->i])
		!= 0 && token->value[data->i] != '\'' && token->value[
			data->i] != '\"' && token->value[data->i] != '$')
	{
		data->i++;
		if (token->value[data->i - 1] == '?')
			break ;
	}
	quote[1] = data->i - s - 1;
	if (quote[0] % 2 == 0)
	{
		str = rep(par->env, &token->value[s + 1], quote, buff);
		if (in_replace(str, s, token, data) == -1)
			return (-1);
	}
	return (0);
}

// find $
int	replace_var(t_token *token, t_data *data, t_params *params)
{
	int		quote[3];

	while (token)
	{
		quote[0] = 0;
		quote[2] = 0;
		data->i = -1;
		while (token->value[++data->i])
		{
			if (token->value[data->i] == '\"')
				quote[2]++;
			if (token->value[data->i] == '\'' && quote[2] % 2 == 0)
				quote[0]++;
			if (token->value[data->i] == '\'' && quote[0] % 2 != 0)
			{
				jump_quotes(token->value, data);
				quote[0]++;
			}
			if (token->value[data->i] == '$')
			{
				if (if_dollar(token, data, params, quote) != 0)
					return (-1);
			}
		}
		token = token->next;
	}
	return (0);
}
