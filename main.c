/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:06:01 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/26 13:03:40 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_st = 0;

// set up error malloc
static int	if_errno(t_params params)
{
	free_params(&params);
	return (12);
}

// main
int	main(int ac, char **av, char **envp)
{
	t_data		data;
	t_params	params;

	if (ac != 1)
		return (ft_printf("This minishell does not take arguments\n"));
	(void)av;
	errno = 0;
	if (isatty(0) == 0 || isatty(1) == 0)
		return (0);
	params.export = NULL;
	params.env = ft_get_env(envp);
	if (errno == 12)
		return (12);
	if (params.env[0] != NULL)
		ft_shlvl(params.env);
	if (errno == 12)
		return (if_errno(params));
	params.export = create_export(params.env);
	if (errno == 12)
		return (if_errno(params));
	print_prompt(&data, &params);
	free_export(params.export);
	free_table(params.env);
	return (g_exit_st);
}