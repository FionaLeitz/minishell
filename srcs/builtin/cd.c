/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:29:22 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/10 09:29:25 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_error_cd(char **arg)
{
	if (arg[1] == NULL)
	{
		ft_printf("minishell: cd: not enough arguments\n");
		return (1);
	}
	if (arg[2] != NULL)
	{
		ft_printf("minishell: cd: too many arguments\n");
		return (1);
	}
	if (chdir(arg[1]) == -1)
	{
		ft_printf("minishell: cd: %s: %s\n", arg[1], strerror(errno));
		return (1);
	}
	return (0);
}

static int	ft_find(char **env, char *str, int i)
{
	int	count;

	count = 0;
	while (env[count] && ft_strncmp(env[count], str, i))
		count++;
	if (env[count] == NULL)
		return (1);
	return (count);
}

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

static int	ft_end_cd(int i, int j, char **env, char *path)
{
	char	*oldpath;

	if (i != -1)
		oldpath = ft_strjoin("OLD", env[i]);
	else
		oldpath = ft_strdup("\0");
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

int	ft_cd(char **arg, char **env)
{
	int		i;
	int		j;
	char	path[4095];

	if (ft_error_cd(arg) == -1)
		return (1);
	i = ft_find(env, "PWD=", 4);
	j = ft_find(env, "OLDPWD=", 7);
	getcwd(path, 4095);
	if (i == -1 && j == -1)
		return (0);
	return (ft_end_cd(i, j, env, path));
}
