/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:57:57 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/24 17:23:32 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// return 0 if c is white space, else return -1
int	ft_space(char c)
{
	if (c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v' || c == ' ')
		return (0);
	return (-1);
}

int	check_fd(int fd)
{
	if (fd == -1)
	{
		perror("Open:");
		exit_st = 127;
	}
	return(-1);
}

int	check_child(int pid)
{
	if (pid < 0)
	{
		perror("Fork:");
		exit_st = 127;
		return (-1);
	}
	return (0);
}

void	ft_shlvl(char** envp)
{
	int	i;
	char 	*shlvl;
	char	*tmp;

	i = 0;
	while (ft_strncmp("SHLVL", *envp, 5))
		envp++;
	shlvl = ft_itoa(ft_atoi(*envp + 6) + 1);
	tmp = NULL;
	while (ft_strncmp("SHLVL", envp[i], 5))
		i++;
	tmp = strndup(envp[i], 6);
	envp[i] = ft_strjoin(tmp, shlvl);
	free(tmp);
}

