/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:57:57 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/13 13:08:12 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// replace quotes when needed to disapear
void	replace_quotes2(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			str[i] = -1;
		else if (str[i] == '\"')
			str[i] = -2;
		i++;
	}
}

// return 0 if c is white space, else return -1
int	ft_space(char c)
{
	if (c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v' || c == ' ')
		return (0);
	return (-1);
}
/*
//increments SHLVL variable
int	ft_shlvl(char **envp)
{
	char	shlvl[12];
	char	*tmp;

	while (*envp && ft_strncmp("SHLVL=", *envp, 6))
		envp++;
	if (!*envp)
		return (-1);
	ft_itoa_no_malloc(ft_atoi(*envp + 6) + 1, shlvl);
	tmp = ft_strndup(*envp, 6);
	if (tmp == NULL)
		return (set_error_malloc("creating env\n"));
	free(*envp);
	*envp = ft_strjoin(tmp, shlvl);
	free(tmp);
	if (*envp == NULL)
		return (set_error_malloc("creating env\n"));
	return (0);
}
*/

// handles ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	write(1, "exit\n", 4);
	write(1, "\n", 1);
	free_params(params);
	free_struct(data);
	exit(g_exit_st);
}

// print different redirection error
int	print_error_redir(char *str, char c)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\'\n", 2);
	return (-1);
}
