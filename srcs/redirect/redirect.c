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

int	get_fd_read(char *pathname, char *red)
{
	int	fd;

	fd = 0;

	if (ft_strcmp(red, "<") == 0)
	{
		fd = open(pathname, O_RDONLY);
		if (fd == -1)
			perror("Error:");
		else
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				perror("Error:");
			close(fd);
		}
	}
	return(fd);
}

int	get_fd_open(char*pathname, char *red)
{
	int	fd;

	fd = 0;//(-1 ?!)
	if (ft_strcmp(red, ">") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else if (ft_strcmp (red, ">>") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
		perror("Error:");
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("Error:");
		close(fd);
	}
	return (fd);
}

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

static int	count_redir(char *value)
{
	int		count;
	char	quote;

	count = 0;
	int i = 0;
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

char **red_tab(t_token *token, t_data *data)
{
	data->i = 0;
	int j;
	int k;
	char *tmp;
	char redi;

	k = count_redir(token->value);
	token->red_tab = malloc(sizeof(char *) * (k + 1));
	if (!token->red_tab)
		return(NULL);
	//printf("k = %d\n", k);
	token->red_tab[k]= NULL;
	while (token->red[data->i])
	{
		j = 0;
		while (token->red[data->i][j])
		{
			if (token->red[data->i][j] == '>' || token->red[data->i][j] == '<')
			{
				redi = token->red[data->i][j];
				tmp = ftt_strchr(&token->red[data->i][j], redi);
				token->red_tab[k] = ft_strdup(tmp);
			}
			j++;
			k++;
		}
		data->i++;

	}
	return(token->red_tab);
}
