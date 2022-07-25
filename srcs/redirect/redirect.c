/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 12:02:03 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/22 12:03:02 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*ftt_strchr(const char *s, int c)
{
	while (*s != '\0' && *s != (char)c)
	{
		s++;
	}
	if (*s == (char)c)
	{
		return ((char *)s + 1);
	}
	return (NULL);
}

int	count_redir(char *value)
{
	int		count;
	int		i;
	char	quote;

	count = 0;
	i = 0;
	while (value[i] != '\0')
	{
		if (value[i] == '\'' || value[i] == '\"')
		{
			quote = value[i];
			i++;
			while (value[i] != quote)
				i++;
		}
		else if (value[i] == '>' || value[i] == '<')
		{
			if (value[i] == value[i + 1])
				i++;
			count++;
		}
		i++;
	}
	return(count);
}

int	red_tab(t_data *data, t_token *token)
{
	data->i = 0;
	int j;
	int k;
	int count;
	char *tmp;
	char redi;

	k = 0;
	count = count_redir(data->trimmed);
	token->red_tab = malloc(sizeof(char *) * (count + 1));
	if (!token->red_tab)
		return(-1);
	token->red_tab[k]= NULL;
	while (token->red[data->i])
	{
		j = 0;
		while (token->red[data->i][j])
		{
			if (token->red[data->i][j] == '>' || token->red[data->i][j] == '<')
			{
				redi = token->red[data->i][j];
				if (token->red[data->i][j] == redi && token->red[data->i][j + 1] == redi)
				{	
					tmp = ftt_strchr(&token->red[data->i][j + 1], redi);
					j++;
				}
				else if (token->red[data->i][j] == redi && token->red[data->i][j + 1] != redi)
					tmp = ftt_strchr(&token->red[data->i][j], redi);
				token->red_tab[k] = ft_strdup(tmp);
				tmp = ft_strtrim(token->red_tab[k], " \t\n\v\f\r");
				token->red_tab[k] = ft_strdup(tmp);
				k++;
			}
			j++;
		}
		data->i++;
	}
	return(0);
}