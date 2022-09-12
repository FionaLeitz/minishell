/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 09:42:04 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/23 10:24:41 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
/*
int	ft_if_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (0);
		i++;
	}
	return (-1);
}
*/
void	print_error_heredoc(char *str, int fd, t_params *params)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document at line delimited by end-of-file", 2);
	ft_putstr_fd(" (wanted '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("')\n", 2);
	free_in_heredoc(params, fd);
	exit(0);
}

// free things in heredoc's child
void	free_in_heredoc(t_params *params, int fd)
{
	close(fd);
	dup2(params->old_fd[0], 0);
	close(params->old_fd[0]);
	free_struct(params->data);
	free_params(params);
}
