/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:03 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 15:08:17 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// print curent working directory
int	ft_pwd(char **arg)
{
	char	*path;

	(void)arg;
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("Minishell: pwd: error retrieving current directory: getcwd: \
		cannot access parent directories");
		return (1);
	}
	else
	{
		ft_printf("%s\n", path);
		free(path);
	}
	return (0);
}
