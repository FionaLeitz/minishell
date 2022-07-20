/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:29:40 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/10 09:29:42 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// TO DO retour en cas d'arguments ? On ne doit pas gerer d'argument
// TO DO ordre d'affichage de env ? Je ne le comprends pas
int	ft_env(char **arg, char **env)
{
	if (arg[1] != NULL)
	{
		ft_printf("env: ‘%s’ too many arguments\n", arg[1]);
		return (1);
	}
	if (env == NULL)
		ft_printf("\n");
	else
		print_table(env);
	return (0);
}

static char	**make_new_env(int count, int l, char **env, char *arg)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (new_env == NULL)
	{
		free_table(env);
		return (NULL);
	}
	count = -1;
	while (env[++count])
		new_env[count] = env[count];
	new_env[count] = ft_strdup(arg);
	if (new_env[count] == NULL)
	{
		free_table(env);
		return (NULL);
	}
	if (new_env[count][l - 2] == '+')
		ft_memmove(&new_env[count][l - 2], &new_env[count][l - 1],
			ft_strlen(&new_env[count][l - 1]) + 1);
	free(env);
	new_env[count + 1] = NULL;
	return (new_env);
}

static char	**if_concat(char *arg, char **env, int count)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * (ft_strlen(arg) + ft_strlen(env[count])));
	if (tmp == NULL)
	{
		free_table(env);
		return (NULL);
	}
	tmp[0] = '\0';
	ft_strcat(tmp, env[count]);
	ft_strcat(tmp, arg);
	free(env[count]);
	env[count] = tmp;
	return (env);
}

static char	**if_replace(char *arg, char **env, int count)
{
	free(env[count]);
	env[count] = ft_strdup(arg);
	if (env[count] == NULL)
	{
		free_table(env);
		return (NULL);
	}
	return (env);
}

char	**new_env(char *arg, char **env)
{
	int		count;
	int		l;

	l = 0;
	while (arg[l] != '\0' && arg[l] != '=')
		l++;
	if (arg[l] == '\0')
		return (env);
	l++;
	count = -1;
	while (env[++count])
	{
		if (ft_strncmp(arg, env[count], l - 2) == 0 && env[count][l - 2] == '='
			&& arg[l - 2] == '+')
			return (if_concat(&arg[l], env, count));
		if (ft_strncmp(arg, env[count], l) == 0)
			return (if_replace(arg, env, count));
	}
	return (make_new_env(count, l, env, arg));
}
