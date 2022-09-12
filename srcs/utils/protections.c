/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protections.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 15:59:41 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 16:00:00 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//fd protection
int	check_fd(int fd, char *red)
{
	if (fd == -1)
	{
		ft_printf("Minishell: %s", red);
		perror(" ");
		g_exit_st = 1;
		return (-1);
	}
	return (0);
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

//errno_malloc
int	set_error_malloc(char *str)
{
	errno = ENOMEM;
	ft_putstr_fd("Out of memory in ", 2);
	ft_putstr_fd(str, 2);
	return (-1);
}

// to return ptr if error malloc (norme)
void	*error_malloc_return(char *str, void *ptr)
{
	set_error_malloc(str);
	return (ptr);
}
