/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:03 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/10 09:30:04 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(char **arg)
{
	char	path[4096];
	char	*path2;

	(void)arg;
	getcwd(path, 4095);
	path2 = ft_strdup(path);
	ft_printf("%s\n", path2);
	free(path2);
	return (0);
}
