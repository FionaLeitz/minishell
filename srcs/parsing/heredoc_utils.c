/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 09:42:04 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/12 13:56:49 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_if_char(char *str, char c)
{
	int	 i;

	i = 0;
	while(str[i])
	{
		if (str[i] == c)
			return (0);
		i++;
	}
	return (-1);
}

/*char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}*/

int	delim_quotes(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if(delim[i] == '\'' || delim[i] == '\"')
			return (1);
	}
	return (0);
}

int	check_delim(char *delim)
{
	int	quotes;

	quotes = 0;
	if (delim_quotes(delim) == 0)
		quotes = 0;
	else
		quotes = 1;
	return (quotes);
}

int	print_error_heredoc(char *str, int fd)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document at line delimited by end-of-file", 2);
	ft_putstr_fd(" (wanted '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("')\n", 2);
	close(fd);
	exit(0);
}

char	*del_quotes_hd(char *delim)
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
	}
	return(delim);
}

int	in_del_quote_hd(char *str, int i)
{
	char	quote;

	quote = str[i];
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	while (str[i] != quote)
		i++;
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	return (i);
}
