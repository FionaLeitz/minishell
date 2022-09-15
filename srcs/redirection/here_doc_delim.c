/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_delim.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 14:37:07 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 14:37:30 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//delete quotes in delimiter
static int	in_del_quote_hd(char *str, int i)
{
	char	quote;

	quote = str[i];
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	while (str[i] != quote)
		i++;
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	return (i);
}

static char	*del_quotes_hd(char *delim)
{
	int	i;

	i = 0;
	while (delim[i] != '\0')
	{
		if (delim[i] == '\'' || delim[i] == '\"')
			i = in_del_quote_hd(delim, i);
		if (delim[i] == '\0')
			break ;
		if (delim[i] == '\'' || delim[i] == '\"')
			i--;
		i++;
	}
	return (delim);
}

//searches for quotes in delimiter
int	delim_quotes(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == '\'' || delim[i] == '\"')
		{
			del_quotes_hd(delim);
			return (1);
		}
		i++;
	}
	return (0);
}
