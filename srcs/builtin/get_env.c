/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:29:55 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/24 17:29:42 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// create environment at the beggining of minishell
char	**ft_get_env(char **envp)
{
	char	**env;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	env = malloc(sizeof(char *) * (count + 1));
	if (env == NULL)
		return (error_malloc_return("creating env\n", NULL));
	count = 0;
	while (envp[count])
	{
		env[count] = ft_strdup(envp[count]);
		if (env[count] == NULL)
		{
			free_table(env);
			return (error_malloc_return("creating env\n", NULL));
		}
		replace_quotes2(env[count]);
		count++;
	}
	env[count] = NULL;
	return (env);
}
