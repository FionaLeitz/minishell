/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:29:22 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 15:06:47 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check arguments and chdir changethe curant working directory
static int	ft_error_cd(char **arg)
{
	if (arg[1] == NULL)
	{
		ft_putstr_fd("minishell: cd: not enough arguments\n", 2);
		return (1);
	}
	if (arg[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (chdir(arg[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg[1], 2);
		perror(": ");
		return (1);
	}
	return (0);
}

// get variable  in env
static int	ft_find(char **env, char *str, int i)
{
	int	count;

	count = 0;
	while (env[count] && ft_strncmp(env[count], str, i))
		count++;
	if (env[count] == NULL)
		return (-1);
	return (count);
}

// end cd correctly
static int	ft_if_cd(int i, int j, char **env)
{
	if (i == -1)
	{
		if (env[j] == NULL)
			return (1);
	}
	else if (j != 1)
	{
		if (env[i] == NULL)
			return (1);
	}
	else if (env[i] == NULL || env[j] == NULL)
	{
		if (env[i] == NULL)
			free(env[i]);
		if (env[j] == NULL)
			free(env[j]);
		return (1);
	}
	return (0);
}

// replace variables in env
static int	ft_end_cd(int i, int j, char **env, char *path)
{
	char	*oldpath;

	if (i != -1)
		oldpath = ft_strjoin("OLD", env[i]);
	else
		oldpath = ft_strdup("\0");
	if (oldpath == NULL)
		return (set_error_malloc("cd\n"));
	if (j != -1)
	{
		free(env[j]);
		env[j] = oldpath;
	}
	if (i != -1)
	{
		free(env[i]);
		env[i] = ft_strjoin("PWD=", path);
	}
	return (ft_if_cd(i, j, env));
}

// change directory
int	ft_cd(char **arg, t_params *params)
{
	int		i;
	int		j;
	char	path[4095];

	if (ft_error_cd(arg) == 1)
	{
		g_exit_st = 1;
		return (1);
	}
	i = ft_find(params->env, "PWD=", 4);
	j = ft_find(params->env, "OLDPWD=", 7);
	if (getcwd(path, 4095) == NULL)
	{
		perror("Minishell: chdir: error retrieving current directory: \
			getcwd: cannot access parent directories");
		return (1);
	}
	if (i == -1 && j == -1)
		return (0);
	return (ft_end_cd(i, j, params->env, path));
}
