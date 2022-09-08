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

int	set_error_malloc(char *str)
{
	errno = ENOMEM;
	ft_putstr_fd("Out of memory in ", 2);
	ft_putstr_fd(str, 2);
	return (-1);
}

// return 0 if c is white space, else return -1
int	ft_space(char c)
{
	if (c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v' || c == ' ')
		return (0);
	return (-1);
}

//fd protection
int	check_fd(int fd, char *red)
{
	if (fd == -1)
	{
		ft_printf("Minishell: %s:", red);
		perror(" ");
		g_exit_st = 1;
	}
	return (-1);
}

//child protection when forking
int	check_child(int pid)
{
	if (pid == -2)
		return (-1);
	if (pid < 0)
	{
		perror("Fork");
		g_exit_st = 127;
		return (-1);
	}
	return (0);
}

//increments SHLVL variable
int	ft_shlvl(char **envp)
{
	int		i;
	char	shlvl[12];
	char	*tmp;

	i = 0;
	if (envp == NULL)
		return (0);
	while (ft_strncmp("SHLVL=", *envp, 6))
		envp++;
	ft_itoa_no_malloc(ft_atoi(*envp + 6) + 1, shlvl);
	tmp = ft_strndup(envp[i], 6);
	if (tmp == NULL)
		return (set_error_malloc("creating env\n"));
	free(envp[i]);
	envp[i] = ft_strjoin(tmp, shlvl);
	free(tmp);
	if (envp[i] == NULL)
		return (set_error_malloc("creating env\n"));
	return (0);
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
