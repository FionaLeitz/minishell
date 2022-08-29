/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 09:42:04 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/23 10:24:41 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_if_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (0);
		i++;
	}
	return (-1);
}

int	delim_quotes(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == '\'' || delim[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	check_delim(char *delim)
{
	int	quotes;

	quotes = 0;
	if (delim_quotes(delim) == 0)
		quotes = 0;
	else if (delim_quotes(delim) == 1)
		quotes = 1;
	return (quotes);
}

void	print_error_heredoc(char *str, int fd)
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
		printf("%d\n", i);
		if (delim[i] == '\0')
			break ;
		if (delim[i] == '\'' || delim[i] == '\"')
			i--;
		i++;
	}
	return (delim);
}

int	in_del_quote_hd(char *str, int i)
{
	char	quote;

	quote = str[i];
	printf("quote = (%c)\n", quote);
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	while (str[i] != quote)
		i++;
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	return (i);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*dest;
	int		i;

	i = 0;
	if (!s1)
	{
		s1 = malloc(sizeof(char) * 1);
		s1[0] = '\0';
	}
	dest = ft_calloc(sizeof(char), (ft_strlen(s1) + 2));
	if (!dest)
		return (NULL);
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = c;
	i++;
	dest[i] = '\0';
	free(s1);
	return (dest);
}
