/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 09:40:11 by fleitz            #+#    #+#             */
/*   Updated: 2021/12/13 15:10:42 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/get_next_line.h"

char	*ft_else(char *buff, char *str, size_t end)
{
	char	*save;
	char	*save2;
	size_t	size;

	save2 = ft_strndup(buff, end);
	save = ft_strjoin(str, save2);
	free(str);
	if (save == NULL)
	{
		if (save2 != NULL)
			free(save2);
		return (NULL);
	}
	size = 0;
	while (save[size])
		size++;
	str = ft_strndup(save, (size + 1));
	free(save);
	free(save2);
	return (str);
}

char	*ft_n_buffcpy(char *buff, char *str)
{
	size_t	end;
	size_t	size;

	end = 0;
	while (buff[end] != '\0' && buff[end] != '\n')
		end++;
	end++;
	if (str == NULL)
		str = ft_strndup(buff, end);
	else
	{
		str = ft_else(buff, str, end);
		if (str == NULL)
			return (NULL);
	}
	size = BUFFER_SIZE - end;
	ft_memcpy(buff, &buff[end], size);
	buff[size] = '\0';
	return (str);
}

char	*ft_buffcpy(char *buff, char *str)
{
	char	*save;

	if (str == NULL)
	{
		str = ft_strndup(buff, BUFFER_SIZE);
		return (str);
	}
	save = ft_strjoin(str, buff);
	free(str);
	buff[0] = '\0';
	return (save);
}

int	ft_stop(char *buff, ssize_t rd)
{
	if (rd == 0)
	{
		buff[1] = 1;
		return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buff[BUFFER_SIZE + 1] = "\0\0";
	char		*str;
	ssize_t		rd;

	str = NULL;
	while (1)
	{
		if (buff[0] == '\0' && buff[1] == 1)
			return (NULL);
		if (ft_strchr(buff, '\n') != NULL)
			return (ft_n_buffcpy(buff, str));
		if (buff[0] != '\0')
		{
			str = ft_buffcpy(buff, str);
			if (str == NULL)
				return (NULL);
		}
		rd = read(fd, buff, BUFFER_SIZE);
		if (rd == -1)
			return (NULL);
		buff[rd] = '\0';
		if (ft_stop(buff, rd) == 0)
			return (str);
	}		
}
