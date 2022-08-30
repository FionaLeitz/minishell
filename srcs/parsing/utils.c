/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:57:57 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/24 17:59:09 by masamoil         ###   ########.fr       */
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

//fd protection
int	check_fd(int fd)
{
	if (fd == -1)
	{
		perror("Open:");
		g_exit_st = 127;
	}
	return (-1);
}

//child protection when forking
int	check_child(int pid)
{
	if (pid < 0)
	{
		perror("Fork:");
		g_exit_st = 127;
		return (-1);
	}
	return (0);
}

//increments SHLVL variable
void	ft_shlvl(char **envp)
{
	int		i;
	char	shlvl[12];
	char	*tmp;

	i = 0;
	if (envp == NULL)
		return ;
	while (ft_strncmp("SHLVL=", *envp, 6))
		envp++;
	ft_itoa_no_malloc(ft_atoi(*envp + 6) + 1, shlvl);
//	tmp = NULL;
//	while (ft_strncmp("SHLVL", envp[i], 5))
//		i++;
	tmp = ft_strndup(envp[i], 6);
	free(envp[i]);
	envp[i] = ft_strjoin(tmp, shlvl);
	free(tmp);
}

// handles ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	write(1, "exit\n", 4);
	write(1, "\n", 1);
	free_params(params);
	free_struct(data);
	exit(g_exit_st);
}
