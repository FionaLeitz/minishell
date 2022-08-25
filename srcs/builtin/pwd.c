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
	char	path[4096];
	char	*path2;

	(void)arg;
	getcwd(path, 4095);
	path2 = ft_strdup(path);
	if (path2 == NULL)
	{
		ft_putstr_fd("error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		return (1);
	}
	else
	{
		ft_printf("%s\n", path2);
		free(path2);
	}
	return (0);
}
