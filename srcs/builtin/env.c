/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:29:40 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 14:53:37 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// if env, print the environment unless arguments
int	ft_env(char **arg, t_params *params)
{
	if (arg[1] != NULL)
	{
		ft_printf("env: ‘%s’: too many arguments\n", arg[1]);
		return (127);
	}
	if (params->env == NULL)
		ft_printf("\n");
	else
		print_table(params->env);
	return (0);
}

// if new variable, recreate environment plus new variable
static int	make_new_env(int count, int l, t_params *params, char *arg)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (new_env == NULL)
	{
		free_table(params->env);
		return (1);
	}
	count = -1;
	while (params->env[++count])
		new_env[count] = params->env[count];
	new_env[count] = ft_strdup(arg);
	if (new_env[count] == NULL)
	{
		free_table(params->env);
		return (1);
	}
	if (new_env[count][l - 2] == '+')
		ft_memmove(&new_env[count][l - 2], &new_env[count][l - 1],
			ft_strlen(&new_env[count][l - 1]) + 1);
	free(params->env);
	new_env[count + 1] = NULL;
	params->env = new_env;
	return (0);
}

// if += variable, concat
static int	if_concat(char *arg, t_params *params, int n)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * (ft_strlen(arg) + ft_strlen(params->env[n])));
	if (tmp == NULL)
	{
		free_table(params->env);
		return (1);
	}
	tmp[0] = '\0';
	ft_strcat(tmp, params->env[n]);
	ft_strcat(tmp, arg);
	free(params->env[n]);
	params->env[n] = tmp;
	return (0);
}

// if variable already exist, replace it
static int	if_replace(char *arg, t_params *params, int count)
{
	free(params->env[count]);
	params->env[count] = ft_strdup(arg);
	if (params->env[count] == NULL)
	{
		free_table(params->env);
		return (1);
	}
	return (0);
}

// if export, make new environment
int	new_env(char *arg, t_params *params)
{
	int		count;
	int		l;
	int		i;

	l = 0;
	while (arg[l] != '\0' && arg[l] != '=')
		l++;
	if (arg[l] == '\0')
		return (0);
	l++;
	i = l;
	while (arg[i])
	{
		if (arg[i] == '\'')
			arg[i] = -1;
		else if (arg[i] == '\"')
			arg[i] = -2;
		i++;
	}
	count = -1;
	while (params->env[++count])
	{
		if (ft_strncmp(arg, params->env[count], l - 2) == 0
			&& params->env[count][l - 2] == '=' && arg[l - 2] == '+')
			return (if_concat(&arg[l], params, count));
		if (ft_strncmp(arg, params->env[count], l) == 0)
			return (if_replace(arg, params, count));
	}
	return (make_new_env(count, l, params, arg));
}
