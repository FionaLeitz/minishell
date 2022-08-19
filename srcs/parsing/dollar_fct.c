/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_fct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 11:42:33 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/03 11:42:39 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// find variable to replace $
char	*rep(char **env, char *str, int *quote, char *buff)
{
	int		count;

	if ((quote[1] == 0 && (ft_space(str[1]) == 0 || str[1] == '\0'))
		|| (quote[0] % 2 != 0 && str[0] == '\"'))
		return ("$");
	if (str[0] == '?' && (ft_space(str[1]) == 0 || str[1] == '\0'))
	{
		ft_itoa_no_malloc(exit_st, buff);
		return (buff);
	}
	count = -1;
	while (env[++count])
	{
		if (ft_strncmp(env[count], str, quote[1]) == 0 && env[count][quote[1]] == '=')
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

// find $
int	replace_var(t_token *token, t_data *data, t_params *params)
{
	int		s;
	char	*str;
	int		quote[2];
	char buff[12];

	while (token)
	{
		quote[0] = 0;
		data->i = -1;
		while (token->value[++data->i])
		{
			if (token->value[data->i] == '\"')
				quote[0]++;
			if (token->value[data->i] == '\'' && quote[0] % 2 == 0)
				jump_quotes(token->value, data);
			if (token->value[data->i] == '$')
			{
				s = data->i++;
				while (token->value[data->i] && ft_space(token->value[data->i])
					!= 0 && token->value[data->i] != '\'' && token->value[
						data->i] != '\"' && token->value[data->i] != '$')
					data->i++;
				quote[1] = data->i - s - 1;
				str = rep(params->env, &token->value[s + 1], quote, buff);
				if (in_replace(str, s, token, data) == -1)
					return (-1);
			}
		}
		token = token->next;
	}
	return (0);
}
